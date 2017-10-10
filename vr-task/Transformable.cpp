#include "Transformable.h"

Transformable::Transformable() :
	m_position(0.0f, 0.0f, 0.0f), m_rotation(0.0f, 0.0f, 0.0f, 1.0f), m_scale(1.0f, 1.0f, 1.0f),
	m_positionChanged(true), m_rotationChanged(true), m_scaleChanged(true)
{
}

mat4 Transformable::getTransformation()
{
	if (m_positionChanged || m_rotationChanged || m_scaleChanged) {
		m_transformation = getPositionMatrix() * getRotationMatrix() * getScaleMatrix();
	}

	return m_transformation;
}

mat4 Transformable::getPositionMatrix()
{
	if (m_positionChanged) {
		m_positionMatrix = glm::translate(mat4(1.0f), m_position);
		m_positionChanged = false;
	}

	return m_positionMatrix;
}

mat4 Transformable::getRotationMatrix()
{
	if (m_rotationChanged) {
		m_rotationMatrix = glm::mat4_cast(m_rotation);
		m_rotationChanged = false;
	}

	return m_rotationMatrix;
}

mat4 Transformable::getScaleMatrix()
{
	if (m_scaleChanged) {
		m_scaleMatrix = glm::scale(mat4(1.0f), m_scale);
		m_scaleChanged = false;
	}

	return m_scaleMatrix;
}

void Transformable::move(float x, float y, float z)
{
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
	m_positionChanged = true;
}

void Transformable::move(const vec3 & vector)
{
	m_position += vector;
	m_positionChanged = true;
}

void Transformable::setPosition(float x, float y, float z)
{
	m_position = vec3(x, y, z);
	m_positionChanged = true;
}

void Transformable::setPosition(const vec3 & position)
{
	m_position = position;
	m_positionChanged = true;
}

vec3 Transformable::getPosition() const
{
	return m_position;
}

void Transformable::rotate(float x, float y, float z)
{
	m_rotation = quat(vec3(x, y, z)) * m_rotation;
	m_rotation = glm::normalize(m_rotation);
	m_rotationChanged = true;
}

void Transformable::rotate(const vec3 & eulerAngles)
{
	m_rotation = quat(eulerAngles) * m_rotation;
	m_rotation = glm::normalize(m_rotation);
	m_rotationChanged = true;
}

void Transformable::setRotation(float x, float y, float z)
{
	m_rotation = quat(vec3(x, y, z));
	m_rotation = glm::normalize(m_rotation);
	m_rotationChanged = true;
}

void Transformable::setRotation(const vec3 & eulerAngles)
{
	m_rotation = quat(eulerAngles);
	m_rotation = glm::normalize(m_rotation);
	m_rotationChanged = true;
}

void Transformable::setRotation(const quat & rotation)
{
	m_rotation = rotation;
	m_rotation = glm::normalize(m_rotation);
	m_rotationChanged = true;
}

quat Transformable::getRotation() const
{
	return m_rotation;
}

void Transformable::scale(float s)
{
	m_scale *= s;
	m_scaleChanged = true;
}

void Transformable::scale(float x, float y, float z)
{
	m_scale *= vec3(x, y, z);
	m_scaleChanged = true;
}

void Transformable::scale(const vec3 & s)
{
	m_scale *= s;
	m_scaleChanged = true;
}

void Transformable::setScale(float s)
{
	m_scale = vec3(s, s, s);
	m_scaleChanged = true;
}

void Transformable::setScale(float x, float y, float z)
{
	m_scale = vec3(x, y, z);
	m_scaleChanged = true;
}

void Transformable::setScale(const vec3 & s)
{
	m_scale = s;
	m_scaleChanged = true;
}

vec3 Transformable::getScale() const
{
	return m_scale;
}

vec3 Transformable::getDirectionFront()
{
	vec4 temp(0.0f, 0.0f, -1.0f, 1.0f);
	temp = m_rotationMatrix * temp;
	temp /= temp.w;
	vec3 result(temp.x, temp.y, temp.z);
	return glm::normalize(result);
}

vec3 Transformable::getDirectionRight()
{
	return glm::normalize(glm::cross(getDirectionFront(), vec3(0.0f, 1.0f, 0.0f)));
}

vec3 Transformable::getDirectionUp()
{
	vec3 directionFront = getDirectionFront();

	return glm::normalize(
		glm::cross(glm::cross(directionFront, vec3(0.0f, 1.0f, 0.0f)), directionFront)
	);
}
