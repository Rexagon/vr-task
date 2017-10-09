#include "BodyRenderer.h"

#include "ResourceManager.h"

#include "ShaderFactory.h"

BodyRenderer::BodyRenderer(Camera * camera) :
	Renderer(camera), m_meshShader(nullptr)
{
	ResourceManager::bind<ShaderFactory>("mesh", "shaders/mesh.vert", "shaders/mesh.frag");

	m_meshShader = ResourceManager::get<Shader>("mesh");

	m_meshShader->setAttribute(0, "vPosition");
	m_meshShader->setAttribute(1, "vTextureCoords");
	m_meshShader->setAttribute(2, "vNormal");

	m_meshShader->bind();
	m_meshShader->setUniform("diffuseTexture", 0);
	m_meshShader->unbind();
}

void BodyRenderer::draw(Body * gameObject)
{
	draw(gameObject, m_meshShader);
}

void BodyRenderer::draw(Body * gameObject, Shader * shader)
{
	if (gameObject == nullptr || gameObject->getMesh() == nullptr ||
		m_camera == nullptr) 
	{
		return;
	}

	shader->bind();

	shader->setUniform("transformation", gameObject->getTransformation());
	shader->setUniform("cameraProjection", m_camera->getProjection());
	shader->setUniform("cameraTransformation", m_camera->getTransformation());

	glActiveTexture(GL_TEXTURE0);
	sf::Texture::bind(gameObject->getDiffuseTexture());

	glEnable(GL_DEPTH_TEST);
	gameObject->getMesh()->draw();
	glDisable(GL_DEPTH_TEST);
	
	sf::Texture::bind(nullptr);

	shader->unbind();
}
