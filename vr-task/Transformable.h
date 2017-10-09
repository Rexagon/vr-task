#pragma once

#include "Math.h"

class Transformable
{
public:
	Transformable();
	virtual ~Transformable() {}

	virtual mat4 getTransformation();

	virtual mat4 getPositionMatrix();
	virtual mat4 getRotationMatrix();
	virtual mat4 getScaleMatrix();

	virtual void move(float x, float y, float z);
	virtual void move(const vec3& vector);
	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(const vec3& position);
	vec3 getPosition() const;

	virtual void rotate(float x, float y, float z);
	virtual void rotate(const vec3& eulerAngles);
	virtual void setRotation(float x, float y, float z);
	virtual void setRotation(const vec3& eulerAngles);
	vec3 getRotation() const;

	void scale(float s);
	void scale(float x, float y, float z);
	void scale(const vec3& s);
	void setScale(float s);
	void setScale(float x, float y, float z);
	void setScale(const vec3& s);
	vec3 getScale() const;

	virtual vec3 getDirectionFront();
	virtual vec3 getDirectionRight();
	virtual vec3 getDirectionUp();

protected:
	mat4 m_transformation;

	mat4 m_positionMatrix;
	mat4 m_rotationMatrix;
	mat4 m_scaleMatrix;

	vec3 m_position;
	vec3 m_rotation;
	vec3 m_scale;

	bool m_positionChanged;
	bool m_rotationChanged;
	bool m_scaleChanged;
};