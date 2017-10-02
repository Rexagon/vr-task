#include "Renderer.h"

Renderer::Renderer() :
	m_camera(nullptr)
{
}

Renderer::Renderer(Camera * camera) :
	m_camera(camera)
{
}

void Renderer::setCamera(Camera * camera)
{
	m_camera = camera;
}

Camera * Renderer::getCamera()
{
	return m_camera;
}
