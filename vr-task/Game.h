#pragma once

#include "Core.h"

class Game : public Scene
{
public:
	void onInit() override;
	void onClose() override;

	void onUpdate(const float dt) override;
	void onDraw(const float dt) override;

	void onResize(const vec2& windowSize) override;

private:
	std::unique_ptr<PerspectiveCamera> m_camera;

	std::unique_ptr<SkyboxRenderer> m_skyboxRenderer;
	std::unique_ptr<GameObjectRenderer> m_gameObjectRenderer;

	GameObject m_shogiban;
	GameObject m_island;
	GameObject m_table;
};