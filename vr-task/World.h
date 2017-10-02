#pragma once

#include <memory>

#include <btBulletDynamicsCommon.h>

#include "Math.h"

class World
{
public:
	World(float g = -9.81f);

private:
	std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
	std::unique_ptr<btCollisionDispatcher> m_dispatcher;
	std::unique_ptr<btDbvtBroadphase> m_broadphase;
	std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
	std::unique_ptr<btDefaultCollisionConfiguration> m_configuration;

	float m_g;
};

vec3 toGLM(const btVector3& vec);
vec4 toGLM(const btVector4& vec);
btVector3 toBT(const vec3& vec);
btVector4 toBT(const vec4& vec);