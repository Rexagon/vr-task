#include "World.h"

World::World(float g)
{
	m_configuration = std::make_unique<btDefaultCollisionConfiguration>();

	m_dispatcher = std::make_unique<btCollisionDispatcher>(m_configuration.get());
	m_broadphase = std::make_unique<btDbvtBroadphase>();
	m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();

	m_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(m_dispatcher.get(), 
		m_broadphase.get(), m_solver.get(), m_configuration.get());

	m_dynamicsWorld->setGravity(btVector3(0.0f, g, 0.0f));
}

vec3 toGLM(const btVector3 & vec)
{
	return vec3(vec.x(), vec.y(), vec.z());
}

vec4 toGLM(const btVector4 & vec)
{
	return vec4(vec.x(), vec.y(), vec.z(), vec.w());
}

btVector3 toBT(const vec3 & vec)
{
	return btVector3(vec.x, vec.y, vec.z);
}

btVector4 toBT(const vec4 & vec)
{
	return btVector4(vec.x, vec.y, vec.z, vec.w);
}
