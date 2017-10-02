#pragma once

#include "Renderer.h"

#include "Cubemap.h"
#include "Shader.h"
#include "Mesh.h"

class SkyboxRenderer : public Renderer
{
public:
	SkyboxRenderer(Camera* camera = nullptr);

	void draw();

	void setSkybox(Cubemap* cubemap);
	Cubemap* getSkybox();

private:
	Mesh* m_cubeMesh;
	Shader* m_skyboxShader;
	Cubemap* m_skyboxCubemap;
};