#pragma once

#include "Renderer.h"

#include "GameObject.h"
#include "Shader.h"

class GameObjectRenderer : public Renderer
{
public:
	GameObjectRenderer(Camera* camera = nullptr);

	void draw(GameObject* gameObject);
	void draw(GameObject* gameObject, Shader* shader);

private:
	Shader* m_meshShader;
};