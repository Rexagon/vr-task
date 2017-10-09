#pragma once

#include <memory>
#include <bitset>
#include <vector>
#include <array>

#include <openvr.h>

#include "Math.h"

using Button = vr::EVRButtonId;
using DeviceIndex = vr::TrackedDeviceIndex_t;

class VRsystem
{
public:
	static void init();
	static void close();

	static void reset();
	static void handleEvents();

	static uvec2 getRenderTargetSize();

	static bool getButton(DeviceIndex device, Button button);
	static bool getButtonDown(DeviceIndex device, Button button);
	static bool getButtonUp(DeviceIndex device, Button button);

	static vec3 getDevicePosition(DeviceIndex device);
	static vec3 getDeviceRotation(DeviceIndex device);
	static mat4 getDeviceTransformation(DeviceIndex device);
	
	static bool isHmdConnected();
	static DeviceIndex getHmdDeviceIndex();

	static size_t getConnectedControllersCount();
	static std::vector<DeviceIndex> getControllerDevicesIndices();

private:
	static void updateControllersInfo();
	static bool processEvent(const vr::VREvent_t& event);

	static const size_t DEVICE_COUNT = vr::k_unMaxTrackedDeviceCount;

	static vr::IVRSystem* m_system;
	
	static std::array<vr::TrackedDevicePose_t, DEVICE_COUNT> m_trackedDevicePoses;
	static std::array<mat4, DEVICE_COUNT> m_trackedDeviceMatrices;

	static std::array<std::bitset<vr::k_EButton_Max>, DEVICE_COUNT> m_currentButtonsState;
	static std::array<std::bitset<vr::k_EButton_Max>, DEVICE_COUNT> m_lastButtonsState;

	static bool m_hmdConnected;
	static DeviceIndex m_hmdDeviceIndex;
	static std::vector<DeviceIndex> m_controllerDevicesIndices;
};

mat4 toGLM(const vr::HmdMatrix34_t& m);