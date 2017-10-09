#pragma once

#include <memory>

#include <btBulletDynamicsCommon.h>

#include "Math.h"

class World
{
public:
	World(float g = -9.81f);

	void setGravity(float g);
	void setGravity(float x, float y, float z);
	void setGravity(const vec3& direction);
	vec3 getGravity() const;



private:
	std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
	std::unique_ptr<btCollisionDispatcher> m_dispatcher;
	std::unique_ptr<btDbvtBroadphase> m_broadphase;
	std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
	std::unique_ptr<btDefaultCollisionConfiguration> m_configuration;

	vec3 m_gravity;
};

vec3 toGLM(const btVector3& vec);
vec4 toGLM(const btVector4& vec);
btVector3 toBT(const vec3& vec);
btVector4 toBT(const vec4& vec);