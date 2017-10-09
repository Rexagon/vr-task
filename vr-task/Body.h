#pragma once

#include <memory>

#include <SFML/Graphics/Texture.hpp>
#include <btBulletDynamicsCommon.h>

#include "Transformable.h"
#include "Mesh.h"

class World;

class Body : public Transformable
{
public:
	virtual ~Body();

	void update(const float dt);

	void move(float x, float y, float z);
	void move(const vec3& vector);
	void setPosition(float x, float y, float z);
	void setPosition(const vec3& position);

	void rotate(float x, float y, float z);
	void rotate(const vec3& eulerAngles);
	void setRotation(float x, float y, float z);
	void setRotation(const vec3& eulerAngles);

	World* getWorld();

	void setMesh(Mesh* mesh, bool updateCollision = false);
	Mesh* getMesh();

	void setDiffuseTexture(sf::Texture* texture);
	sf::Texture* getDiffuseTexture();

	float getMass() const;

	void setFriction(float friction);
	float getFriction() const;

	btRigidBody* getPhysicsData();

	void setCollisionShape(btCollisionShape* collisionShape);
	btCollisionShape* getCollisionShape();

private:
	friend class World;

	Body(World* world, Mesh* mesh, float mass = 0.0f);

	void updatePhysicsPosition();
	void updatePhysicsRotation();

	World* m_world;

	Mesh* m_mesh;

	sf::Texture* m_diffuseTexture;

	std::unique_ptr<btRigidBody> m_physicsData;
	float m_mass;
};