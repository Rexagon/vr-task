#include "SkyboxRenderer.h"

#include <vector>

#include "ResourceManager.h"

#include "MeshFactory.h"
#include "ShaderFactory.h"
#include "CubemapFactory.h"

SkyboxRenderer::SkyboxRenderer(Camera* camera) :
	Renderer(camera),
	m_cubeMesh(nullptr), m_skyboxShader(nullptr), m_skyboxCubemap(nullptr)
{
	ResourceManager::bind<MeshFactory>("skybox", "models/cube.obj");
	ResourceManager::bind<ShaderFactory>("skybox", "shaders/skybox.vert", "shaders/skybox.frag");
	ResourceManager::bind<CubemapFactory>("skybox", std::vector<std::string>({
		"textures/posx.png",
		"textures/negx.png",
		"textures/posy.png",
		"textures/negy.png",
		"textures/posz.png",
		"textures/negz.png"
	}));

	m_cubeMesh = ResourceManager::get<Mesh>("skybox");
	m_skyboxShader = ResourceManager::get<Shader>("skybox");
	m_skyboxCubemap = ResourceManager::get<Cubemap>("skybox");

	m_skyboxShader->setAttribute(0, "vPosition");
	m_skyboxShader->setAttribute(1, "vTextureCoords");

	m_skyboxShader->bind();
	m_skyboxShader->setUniform("diffuseTexture", 0);
	m_skyboxShader->unbind();
}

void SkyboxRenderer::draw()
{
	if (m_camera == nullptr || m_skyboxCubemap == nullptr) {
		return;
	}

	m_skyboxShader->bind();

	m_skyboxShader->setUniform("cameraRotation", m_camera->getRotationMatrix());
	m_skyboxShader->setUniform("cameraProjection", m_camera->getProjection());

	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);

	m_skyboxCubemap->bind(0);
	m_cubeMesh->draw();
	m_skyboxCubemap->unbind();

	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	m_skyboxShader->unbind();
}

void SkyboxRenderer::setSkybox(Cubemap * cubemap)
{
	m_skyboxCubemap = cubemap;
}

Cubemap * SkyboxRenderer::getSkybox()
{
	return m_skyboxCubemap;
}
