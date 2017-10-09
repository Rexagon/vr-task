#include "Body.h"

Body::Body() :
	m_mesh(nullptr)
{
	btCollisionShape* collisionShape = new btEmptyShape();
}

Body::Body(Mesh * mesh) :
	m_mesh(mesh)
{
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
