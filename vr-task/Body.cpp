#include "Body.h"

#include "World.h"

Body::Body(World* world, Mesh * mesh, float mass) :
	m_world(world), m_mesh(mesh), m_mass(mass)
{
	btCollisionShape* collisionShape;

	if (m_mesh == nullptr) {
		collisionShape = new btEmptyShape();
	}
	else {
		std::vector<float> positions(3 * m_mesh->getPositionsNum());
		GLuint vbo = m_mesh->getPositionsBuffer();
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(float), &positions[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		collisionShape = new btConvexHullShape(&positions[0], m_mesh->getPositionsNum(), sizeof(float) * 3);
	}

	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(
		btQuaternion(0, 0, 0, 1),
		btVector3(0, 0, 0)
	));

	btVector3 inertia;
	collisionShape->calculateLocalInertia(m_mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(
		m_mass,
		motionState,
		collisionShape,
		inertia
	);

	m_physicsData = std::make_unique<btRigidBody>(rigidBodyInfo);
}

Body::~Body()
{
	delete m_physicsData->getMotionState();
	delete m_physicsData->getCollisionShape();
	m_physicsData.reset(nullptr);
}

void Body::update(const float dt)
{
	btTransform transform = m_physicsData->getCenterOfMassTransform();
	vec3 position = toGLM(transform.getOrigin());
	if (position != m_position) {
		m_position = position;
		m_positionChanged = true;
	}

	m_physicsData->getMotionState()->getWorldTransform(transform);
	quat rotation = toGLM(transform.getRotation());
	if (rotation != m_rotation) {
		m_rotation = rotation;
		m_rotationChanged = true;
	}
}

void Body::move(float x, float y, float z)
{
	Transformable::move(x, y, z);
	updatePhysicsPosition();
}

void Body::move(const vec3 & vector)
{
	Transformable::move(vector);
	updatePhysicsPosition();
}

void Body::setPosition(float x, float y, float z)
{
	Transformable::setPosition(x, y, z);
	updatePhysicsPosition();
}

void Body::setPosition(const vec3 & position)
{
	Transformable::setPosition(position);
	updatePhysicsPosition();
}

void Body::rotate(float x, float y, float z)
{
	Transformable::rotate(x, y, z);
	updatePhysicsRotation();
}

void Body::rotate(const vec3 & eulerAngles)
{
	Transformable::rotate(eulerAngles);
	updatePhysicsRotation();
}

void Body::setRotation(float x, float y, float z)
{
	Transformable::setRotation(x, y, z);
	updatePhysicsRotation();
}

void Body::setRotation(const vec3 & eulerAngles)
{
	Transformable::setRotation(eulerAngles);
	updatePhysicsRotation();
}

World * Body::getWorld()
{
	return m_world;
}

void Body::setMesh(Mesh * mesh, bool updateCollision)
{
	m_mesh = mesh;
}

Mesh * Body::getMesh()
{
	return m_mesh;
}

void Body::setDiffuseTexture(sf::Texture * texture)
{
	m_diffuseTexture = texture;
}

sf::Texture * Body::getDiffuseTexture()
{
	return m_diffuseTexture;
}

float Body::getMass() const
{
	return m_mass;
}

void Body::setFriction(float friction)
{
	m_physicsData->setFriction(friction);
}

float Body::getFriction() const
{
	return m_physicsData->getFriction();
}

btRigidBody * Body::getPhysicsData()
{
	return m_physicsData.get();
}

void Body::setCollisionShape(btCollisionShape * collisionShape)
{
	m_physicsData->setCollisionShape(collisionShape);
}

btCollisionShape * Body::getCollisionShape()
{
	return m_physicsData->getCollisionShape();
}

void Body::updatePhysicsPosition()
{
	btTransform transform = m_physicsData->getCenterOfMassTransform();
	transform.setOrigin(toBT(m_position));
	m_physicsData->setCenterOfMassTransform(transform);
}

void Body::updatePhysicsRotation()
{
	btTransform worldTransform;
	m_physicsData->getMotionState()->getWorldTransform(worldTransform);
	worldTransform.setRotation(btQuaternion(m_rotation.z, m_rotation.y, m_rotation.x));
	m_physicsData->getMotionState()->setWorldTransform(worldTransform);
}
