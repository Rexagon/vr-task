#include "VRsystem.h"

#include "Log.h"

vr::IVRSystem* VRsystem::m_system = nullptr;
std::array<vr::TrackedDevicePose_t, vr::k_unMaxTrackedDeviceCount> VRsystem::m_trackedDevicePoses;
std::array<mat4, vr::k_unMaxTrackedDeviceCount> VRsystem::m_trackedDeviceMatrices;
std::array<std::bitset<vr::k_EButton_Max>, VRsystem::DEVICE_COUNT> VRsystem::m_currentButtonsState;
std::array<std::bitset<vr::k_EButton_Max>, VRsystem::DEVICE_COUNT> VRsystem::m_lastButtonsState;
bool VRsystem::m_hmdConnected = false;
DeviceIndex VRsystem::m_hmdDeviceIndex = 0;
std::vector<DeviceIndex> VRsystem::m_controllerDevicesIndices;

void VRsystem::init()
{
	vr::EVRInitError error = vr::VRInitError_None;
	m_system = vr::VR_Init(&error, vr::VRApplication_Scene);

	if (error != vr::VRInitError_None) {
		throw std::runtime_error("Unable to initialize VR");
	}

	updateControllersInfo();
}

void VRsystem::close()
{
	if (m_system) {
		vr::VR_Shutdown();
		m_system = nullptr;
	}
}

void VRsystem::reset()
{
	for (size_t i = 0; i < DEVICE_COUNT; ++i) {
		m_lastButtonsState[i] = m_currentButtonsState[i];
	}
}

void VRsystem::handleEvents()
{
	vr::VRCompositor()->WaitGetPoses(&m_trackedDevicePoses[0], vr::k_unMaxTrackedDeviceCount, NULL, 0);

	vr::VREvent_t event;
	while (m_system->PollNextEvent(&event, sizeof(event))) {
		processEvent(event);
	}

	for (DeviceIndex i = 0; i < DEVICE_COUNT; ++i) {
		if (!m_system->IsTrackedDeviceConnected(i)) {
			continue;
		}

		vr::TrackedDevicePose_t& devicePose = m_trackedDevicePoses[i];

		vr::ETrackedDeviceClass deviceClass = vr::VRSystem()->GetTrackedDeviceClass(i);
		switch (deviceClass)
		{
		case vr::TrackedDeviceClass_HMD:
		case vr::TrackedDeviceClass_Controller:
		{
			vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0, &devicePose, 1);

			m_trackedDeviceMatrices[i] = toGLM(devicePose.mDeviceToAbsoluteTracking);
		}
			break;

		default:
			break;
		}
	}
}

uvec2 VRsystem::getRenderTargetSize()
{
	uint32_t width, height;
	m_system->GetRecommendedRenderTargetSize(&width, &height);
	return uvec2(width, height);
}

bool VRsystem::getButton(DeviceIndex device, Button button)
{
	return m_currentButtonsState[device][button];
}

bool VRsystem::getButtonDown(DeviceIndex device, Button button)
{
	return m_currentButtonsState[device][button] && 
		!m_lastButtonsState[device][button];
}

bool VRsystem::getButtonUp(DeviceIndex device, Button button)
{
	return !m_currentButtonsState[device][button] && 
		m_lastButtonsState[device][button];
}

vec3 VRsystem::getDevicePosition(DeviceIndex device)
{
	const mat4& m = m_trackedDeviceMatrices[device];
	vec3 result;
	result.x = m[0][3];
	result.y = m[1][3];
	result.z = m[2][3];
	return result;
}

quat VRsystem::getDeviceRotation(DeviceIndex device)
{
	const mat4& m = m_trackedDeviceMatrices[device];

	quat rotation;
	rotation.w = sqrt(fmax(0.0f, 1.0f + m[0][0] + m[1][1] + m[2][2])) / 2.0f;
	rotation.x = sqrt(fmax(0.0f, 1.0f + m[0][0] - m[1][1] - m[2][2])) / 2.0f;
	rotation.y = sqrt(fmax(0.0f, 1.0f - m[0][0] + m[1][1] - m[2][2])) / 2.0f;
	rotation.z = sqrt(fmax(0.0f, 1.0f - m[0][0] - m[1][1] + m[2][2])) / 2.0f;

	rotation.x = copysign(rotation.x, m[2][1] - m[1][2]);
	rotation.y = copysign(rotation.y, m[0][2] - m[2][0]);
	rotation.z = copysign(rotation.z, m[1][0] - m[0][1]);

	return rotation;
}

mat4 VRsystem::getDeviceTransformation(DeviceIndex device)
{
	return m_trackedDeviceMatrices[device];
}

bool VRsystem::isHmdConnected()
{
	return m_hmdConnected;
}

DeviceIndex VRsystem::getHmdDeviceIndex()
{
	return m_hmdDeviceIndex;
}

size_t VRsystem::getConnectedControllersCount()
{
	return m_controllerDevicesIndices.size();
}

std::vector<DeviceIndex> VRsystem::getControllerDevicesIndices()
{
	return m_controllerDevicesIndices;
}

void VRsystem::updateControllersInfo()
{
	m_hmdConnected = false;
	m_controllerDevicesIndices.clear();

	for (uint32_t i = 0; i < vr::k_unMaxTrackedDeviceCount; i++)
	{
		vr::TrackedDeviceClass deviceClass = vr::VRSystem()->GetTrackedDeviceClass(i);

		switch (deviceClass)
		{
		case vr::TrackedDeviceClass_Controller:
			m_controllerDevicesIndices.push_back(i);
			Log::write("Controller device id:", i);
		break;

		case vr::TrackedDeviceClass_HMD:
			m_hmdConnected = true;
			m_hmdDeviceIndex = i;
			Log::write("HMD device id:", i);
			break;

		case vr::TrackedDeviceClass_Invalid:
			Log::write("Invalid device id:", i);
			break;
		}
	}
}

bool VRsystem::processEvent(const vr::VREvent_t & event)
{
	switch (event.eventType) {
	case vr::VREvent_TrackedDeviceActivated:
		Log::write("Device activated.", event.trackedDeviceIndex);
		updateControllersInfo();
		break;

	case vr::VREvent_TrackedDeviceDeactivated:
		Log::write("Device deactivated.", event.trackedDeviceIndex);
		updateControllersInfo();
		break;

	case vr::VREvent_ButtonPress:
		m_currentButtonsState[event.trackedDeviceIndex].set(event.data.controller.button, true);
		break;

	case vr::VREvent_ButtonUnpress:
		m_currentButtonsState[event.trackedDeviceIndex].set(event.data.controller.button, false);
		break;
	}

	return true;
}

mat4 toGLM(const vr::HmdMatrix34_t & m)
{
	mat4 result(1.0f);

	result[0][0] = m.m[0][0];
	result[1][0] = m.m[1][0];
	result[2][0] = m.m[2][0];

	result[0][1] = m.m[0][1];
	result[1][1] = m.m[1][1];
	result[2][1] = m.m[2][1];

	result[0][2] = m.m[0][2];
	result[1][2] = m.m[1][2];
	result[2][2] = m.m[2][2];

	result[0][3] = m.m[0][3];
	result[1][3] = m.m[1][3];
	result[2][3] = m.m[2][3];

	return result;
}
