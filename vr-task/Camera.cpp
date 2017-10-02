#include "Camera.h"

#include "Log.h"

Camera::Camera() :
	m_zRange(0.1f, 100.0f), m_projectionChanged(true)
{
	//m_rotation = glm::eulerAngles(quat(0, 0, 0, -1.0f));
	Log::write(m_rotation);
}

Camera::Camera(float nearZ, float farZ) :
	m_zRange(nearZ, farZ), m_projectionChanged(true)
{
	//m_rotation = glm::eulerAngles(quat(0, 0, 0, -1.0f));
	Log::write(m_rotation);
}

Camera::Camera(const vec2& zRange) :
	m_zRange(zRange)
{
	//m_rotation = glm::eulerAngles(quat(0, 0, 0, -1.0f));
	Log::write(m_rotation);
}

mat4 Camera::getProjection()
{
	if (m_projectionChanged) {
		updateProjection();
		m_projectionChanged = false;
	}

	return m_projection;
}

mat4 Camera::getTransformation()
{
	if (m_positionChanged || m_rotationChanged || m_scaleChanged) {
		m_transformation = getScaleMatrix() * getRotationMatrix() * getPositionMatrix();
	}

	return m_transformation;
}

mat4 Camera::getPositionMatrix()
{
	if (m_positionChanged) {
		m_positionMatrix = glm::translate(mat4(1.0f), -m_position);
		m_positionChanged = false;
	}

	return m_positionMatrix;
}

mat4 Camera::getRotationMatrix()
{
	if (m_rotationChanged) {
		m_rotationMatrix = glm::mat4_cast(quat(-m_rotation));
		m_rotationChanged = false;
	}

	return m_rotationMatrix;
}

mat4 Camera::getScaleMatrix()
{
	if (m_scaleChanged) {
		m_scaleMatrix = glm::scale(mat4(1.0f), 1.0f / m_scale);
		m_scaleChanged = false;
	}

	return m_scaleMatrix;
}

void Camera::setNearZ(float nearZ)
{
	m_zRange.x = nearZ;
	m_projectionChanged = true;
}

float Camera::getNearZ() const
{
	return m_zRange.x;
}

void Camera::setFarZ(float farZ)
{
	m_zRange.y = farZ;
	m_projectionChanged = true;
}

float Camera::getFarZ() const
{
	return m_zRange.y;
}

void Camera::setZRange(const vec2 & zRange)
{
	m_zRange = zRange;
	m_projectionChanged = true;
}

vec2 Camera::getZRange()
{
	return m_zRange;
}


//
PerspectiveCamera::PerspectiveCamera(float fov, float aspect) :
	m_fov(fov), m_aspect(aspect), Camera()
{
}

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float nearZ, float farZ) :
	m_fov(fov), m_aspect(aspect), Camera(nearZ, farZ)
{
}

void PerspectiveCamera::setFOV(float fov)
{
	m_fov = fov;
	m_projectionChanged = true;
}

float PerspectiveCamera::getFOV() const
{
	return m_fov;
}

void PerspectiveCamera::setAspect(float aspect)
{
	m_aspect = aspect;
	m_projectionChanged = true;
}

float PerspectiveCamera::getAspect() const
{
	return m_aspect;
}

void PerspectiveCamera::updateProjection()
{
	m_projection = glm::perspective(m_fov, m_aspect, m_zRange.x, m_zRange.y);
}


//
IsometricCamera::IsometricCamera(float leftAndRight, float bottomAndTop) :
	m_left(-leftAndRight), m_right(leftAndRight), m_bottom(-bottomAndTop), m_top(bottomAndTop), Camera()
{
}

IsometricCamera::IsometricCamera(float left, float right, float bottom, float top) :
	m_left(left), m_right(right), m_bottom(bottom), m_top(top), Camera()
{
}

IsometricCamera::IsometricCamera(float left, float right, float bottom, float top, float nearZ, float farZ) :
	m_left(left), m_right(right), m_bottom(bottom), m_top(top), Camera(nearZ, farZ)
{
}

void IsometricCamera::setDimensions(float leftAndRight, float bottomAndTop)
{
	m_left = -leftAndRight;
	m_right = leftAndRight;
	m_bottom = -bottomAndTop;
	m_top = bottomAndTop;
	m_projectionChanged = true;
}

void IsometricCamera::setDimensions(float left, float right, float bottom, float top)
{
	m_left = left;
	m_right = right;
	m_bottom = bottom;
	m_top = top;
	m_projectionChanged = true;
}

float IsometricCamera::getLeftDimension() const
{
	return m_left;
}

float IsometricCamera::getRightDimension() const
{
	return m_right;
}

float IsometricCamera::getBottomDimension() const
{
	return m_bottom;
}

float IsometricCamera::getTopDimension() const
{
	return m_top;
}

void IsometricCamera::updateProjection()
{
	m_projection = glm::ortho(m_left, m_right, m_bottom, m_top, m_zRange.x, m_zRange.y);
}
