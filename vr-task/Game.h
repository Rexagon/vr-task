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
	void initResources();
	void initBoard();

	void drawScene();

	std::unique_ptr<SkyboxRenderer> m_skyboxRenderer;
	std::unique_ptr<BodyRenderer> m_bodyRenderer;

	World m_world;

	Body* m_shogiban;
	Body* m_island;
	Body* m_table;

	std::vector<Body*> m_figures;

#ifdef VR_ENABLED

	std::unique_ptr<VRCamera> m_leftEye;
	std::unique_ptr<VRCamera> m_rightEye;

	std::unique_ptr<FrameBuffer> m_leftEyeBuffer;
	std::unique_ptr<FrameBuffer> m_rightEyeBuffer;

	sf::RectangleShape m_leftEyeDebugSprite;
	sf::RectangleShape m_rightEyeDebugSprite;

#else

	std::unique_ptr<PerspectiveCamera> m_camera;

#endif // VR_ENABLED
};