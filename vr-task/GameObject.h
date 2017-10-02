#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "Transformable.h"
#include "Mesh.h"

class GameObject : public Transformable
{
public:
	GameObject();
	GameObject(Mesh* mesh);

	void setMesh(Mesh* mesh);
	Mesh* getMesh();

	void setDiffuseTexture(sf::Texture* texture);
	sf::Texture* getDiffuseTexture();

private:
	Mesh* m_mesh;

	sf::Texture* m_diffuseTexture;
};