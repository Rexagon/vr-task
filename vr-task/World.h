#pragma once

#include <memory>
#include <vector>

#include <btBulletDynamicsCommon.h>

#include "Body.h"
#include "Math.h"

class World
{
public:
	World(float g = -9.81f);
	~World();

	void update(const float dt);

	Body* createBody(Mesh* mesh, float mass = 0.0f);

	void setGravity(float g);
	void setGravity(float x, float y, float z);
	void setGravity(const vec3& direction);
	vec3 getGravity() const;

	void removeBody(const Body* body);
	bool hasBody(const Body* body) const;

	void addPhysicsData(btRigidBody* physicsData);
	void updatePhysicsData(btRigidBody* physicsData);
	void removePhysicsData(btRigidBody* physicsData);

private:
	std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
	btBroadphaseInterface* m_broadphase;
	btDefaultCollisionConfiguration* m_configuration;
	btCollisionDispatcher* m_dispatcher;
	btSequentialImpulseConstraintSolver* m_solver;

	vec3 m_gravity;

	std::vector<std::unique_ptr<Body>> m_bodies;
};

vec3 toGLM(const btVector3& vec);
vec4 toGLM(const btVector4& vec);
btVector3 toBT(const vec3& vec);
btVector4 toBT(const vec4& vec);