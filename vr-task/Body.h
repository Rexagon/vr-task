#pragma once

#include <memory>

#include <SFML/Graphics/Texture.hpp>
#include <btBulletDynamicsCommon.h>

#include "Transformable.h"
#include "Mesh.h"

class Body : public Transformable
{
public:
	Body();
	Body(Mesh* mesh);

	void setMesh(Mesh* mesh, bool updateCollision = false);
	Mesh* getMesh();

	void setDiffuseTexture(sf::Texture* texture);
	sf::Texture* getDiffuseTexture();

	void setMass(float mass);
	float getMass() const;

	void setCollisionShape(btCollisionShape* collisionShape);

private:
	Mesh* m_mesh;

	sf::Texture* m_diffuseTexture;
	std::unique_ptr<btRigidBody> m_physicsData;
};