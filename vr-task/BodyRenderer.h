#pragma once

#include "Renderer.h"

#include "Shader.h"
#include "Body.h"

class BodyRenderer : public Renderer
{
public:
	BodyRenderer(Camera* camera = nullptr);

	void draw(Body* gameObject);
	void draw(Body* gameObject, Shader* shader);

private:
	Shader* m_meshShader;
};