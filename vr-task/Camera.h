#pragma once

#include "VRsystem.h"

#include "Transformable.h"

class Camera : public Transformable
{
public:
	Camera();
	Camera(float nearZ, float farZ);
	Camera(const vec2& zRange);
	virtual ~Camera() {}
	
	mat4 getProjection();

	mat4 getTransformation() override;

	mat4 getPositionMatrix() override;
	mat4 getRotationMatrix() override;
	mat4 getScaleMatrix() override;

	void setNearZ(float nearZ);
	float getNearZ() const;

	void setFarZ(float farZ);
	float getFarZ() const;

	void setZRange(const vec2& zRange);
	vec2 getZRange();

	vec3 getDirectionFront() override;
	vec3 getDirectionRight() override;
	vec3 getDirectionUp() override;

protected:
	virtual void updateProjection() = 0;

	mat4 m_projection;

	vec2 m_zRange;

	bool m_projectionChanged;
};


class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(float fov, float aspect);
	PerspectiveCamera(float fov, float aspect, float nearZ, float farZ);

	void setFOV(float fov);
	float getFOV() const;

	void setAspect(float aspect);
	float getAspect() const;

private:
	void updateProjection() override;

	float m_aspect;
	float m_fov;
};


class IsometricCamera : public Camera
{
public:
	IsometricCamera(float leftAndRight, float bottomAndTop);
	IsometricCamera(float left, float right, float bottom, float top);
	IsometricCamera(float left, float right, float bottom, float top, float nearZ, float farZ);

	void setDimensions(float leftAndRight, float bottomAndTop);
	void setDimensions(float left, float right, float bottom, float top);

	float getLeftDimension() const;
	float getRightDimension() const;
	float getBottomDimension() const;
	float getTopDimension() const;

private:
	void updateProjection() override;

	float m_left;
	float m_right;
	float m_bottom;
	float m_top;
};


class VRCamera : public Camera
{
public:
	VRCamera(vr::EVREye eye);
	VRCamera(vr::EVREye eye, float nearZ, float farZ);

	void updateProjection() override;

private:
	vr::EVREye m_eye;
};