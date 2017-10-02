#include "Math.h"

quat createRotation(const vec3 & axis, float angle)
{
	quat result;
	float halfAngle = angle / 2.0f;
	result.w = cos(halfAngle);
	result.x = axis.x * sin(halfAngle);
	result.y = axis.y * sin(halfAngle);
	result.z = axis.z * sin(halfAngle);
	return result;
}
