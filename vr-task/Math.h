#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

using vec2 = glm::vec2;
using ivec2 = glm::ivec2;
using uvec2 = glm::uvec2;

using vec3 = glm::vec3;
using ivec3 = glm::ivec3;
using uvec3 = glm::uvec3;

using vec4 = glm::vec4;
using ivec4 = glm::ivec4;
using uvec4 = glm::uvec4;

using mat4 = glm::mat4;

using quat = glm::quat;

quat createRotation(const vec3& axis, float angle);