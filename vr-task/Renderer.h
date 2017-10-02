#pragma once

#include "Camera.h"

class Renderer
{
public:
	Renderer();
	Renderer(Camera* camera);

	void setCamera(Camera* camera);
	Camera* getCamera();

protected:
	Camera* m_camera;
};