#include "GameObject.h"

GameObject::GameObject() :
	m_mesh(nullptr)
{
}

GameObject::GameObject(Mesh * mesh) :
	m_mesh(mesh)
{
}

void GameObject::setMesh(Mesh * mesh)
{
	m_mesh = mesh;
}

Mesh * GameObject::getMesh()
{
	return m_mesh;
}

void GameObject::setDiffuseTexture(sf::Texture * texture)
{
	m_diffuseTexture = texture;
}

sf::Texture * GameObject::getDiffuseTexture()
{
	return m_diffuseTexture;
}
