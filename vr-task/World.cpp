#include "World.h"

#include "Body.h"

World::World(float g)
{
	m_broadphase = new btDbvtBroadphase();

	m_configuration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_configuration);
	m_solver = new btSequentialImpulseConstraintSolver();

	m_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(m_dispatcher,
		m_broadphase, m_solver, m_configuration);

	m_dynamicsWorld->setGravity(btVector3(0.0f, g, 0.0f));
}

World::~World()
{
	m_dynamicsWorld.reset(nullptr);
	delete m_broadphase;
	delete m_configuration;
	delete m_dispatcher;
	delete m_solver;
}

void World::update(const float dt)
{
	m_dynamicsWorld->stepSimulation(dt, 2);

	for (auto& body : m_bodies) {
		body->update(dt);
	}
}

Body * World::createBody(Mesh * mesh, float mass)
{
	std::unique_ptr<Body> body = std::unique_ptr<Body>(new Body(this, mesh, mass));

	addPhysicsData(body->getPhysicsData());

	Body* result = body.get();
	m_bodies.push_back(std::move(body));
	return result;
}

void World::setGravity(float g)
{
	m_dynamicsWorld->setGravity(btVector3(0.0f, g, 0.0f));
	m_gravity = vec3(0.0f, g, 0.0f);
}

void World::setGravity(float x, float y, float z)
{
	m_dynamicsWorld->setGravity(btVector3(x, y, z));
	m_gravity = vec3(x, y, z);
}

void World::setGravity(const vec3 & direction)
{
	m_dynamicsWorld->setGravity(btVector3(direction.x, direction.y, direction.z));
	m_gravity = direction;
}

vec3 World::getGravity() const
{
	return m_gravity;
}

void World::removeBody(const Body * body)
{
	for (auto it = m_bodies.begin(); it != m_bodies.end(); ++it) {
		if (it->get() == body) {
			removePhysicsData((*it)->getPhysicsData());
			m_bodies.erase(it);
		}
	}
}

bool World::hasBody(const Body * body) const
{
	for (const auto& b : m_bodies) {
		if (b.get() == body) {
			return true;
		}
	}
	return false;
}

void World::addPhysicsData(btRigidBody * physicsData)
{
	m_dynamicsWorld->addRigidBody(physicsData);
}

void World::updatePhysicsData(btRigidBody * physicsData)
{
	removePhysicsData(physicsData);
	addPhysicsData(physicsData);
}

void World::removePhysicsData(btRigidBody * physicsData)
{
	m_dynamicsWorld->removeRigidBody(physicsData);
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
