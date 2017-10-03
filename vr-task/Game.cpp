#include "Game.h"

#include "Core.h"

void Game::onInit()
{
	ResourceManager::bind<MeshFactory>("shogiban", "models/shogiban.obj");
	ResourceManager::bind<MeshFactory>("island", "models/island.obj");
	ResourceManager::bind<MeshFactory>("table", "models/table.obj");

	ResourceManager::bind<TextureFactory>("shogiban", "textures/shogiban.png");
	ResourceManager::bind<TextureFactory>("island", "textures/island.png");
	ResourceManager::bind<TextureFactory>("table", "textures/table.png");

	m_eyeTexture.create(1080, 1200, true);
	m_eyeTexture.setActive(true);

	m_leftEye = std::make_unique<VRCamera>(vr::Eye_Left);
	m_rightEye = std::make_unique<VRCamera>(vr::Eye_Right);

	m_skyboxRenderer = std::make_unique<SkyboxRenderer>();
	m_gameObjectRenderer = std::make_unique<GameObjectRenderer>();

	m_shogiban.setMesh(ResourceManager::get<Mesh>("shogiban"));
	m_shogiban.setDiffuseTexture(ResourceManager::get<sf::Texture>("shogiban"));
	m_shogiban.getDiffuseTexture()->setSmooth(true);

	m_island.setMesh(ResourceManager::get<Mesh>("island"));
	m_island.setDiffuseTexture(ResourceManager::get<sf::Texture>("island"));
	m_island.getDiffuseTexture()->setSmooth(true);
	m_island.setPosition(0.0f, -10.0f, 0.0f);
	
	m_table.setMesh(ResourceManager::get<Mesh>("table"));
	m_table.setDiffuseTexture(ResourceManager::get<sf::Texture>("table"));
	m_table.getDiffuseTexture()->setSmooth(true);

}

void Game::onClose()
{
	m_skyboxRenderer.reset(nullptr);
	m_gameObjectRenderer.reset(nullptr);
}

void Game::onUpdate(const float dt)
{
	if (Input::getKeyDown(Key::Escape)) {
		SceneManager::deleteScene();
		return;
	}

	/*vec3 vec(0.0f, 0.0f, 0.0f);
	if (Input::getKey(Key::W)) {
		vec += m_camera->getDirectionFront();
	}
	else if (Input::getKey(Key::S)) {
		vec -= m_camera->getDirectionFront();
	}

	if (Input::getKey(Key::D)) {
		vec += m_camera->getDirectionRight();
	}
	else if (Input::getKey(Key::A)) {
		vec -= m_camera->getDirectionRight();
	}

	if (Input::getKey(Key::Space)) {
		vec += m_camera->getDirectionUp();
	}
	else if (Input::getKey(Key::C)) {
		vec -= m_camera->getDirectionUp();
	}

	m_camera->move(vec * dt * 20.0f);

	vec2 mouseDelta = Input::getMouseDeltaPosition();
	if (mouseDelta != vec2()) {
		m_camera->rotate(0.0f, -mouseDelta.x * dt, 0.0f);
		m_camera->rotate(-mouseDelta.y * dt, 0.0f, 0.0f);
	}*/

	m_leftEye->setPosition(VRsystem::getDevicePosition(0) * 10.0f);
	m_leftEye->setRotation(VRsystem::getDeviceRotation(0));

	m_rightEye->setPosition(VRsystem::getDevicePosition(0) * 10.0f);
	m_rightEye->setRotation(VRsystem::getDeviceRotation(0));
}

void Game::onDraw(const float dt)
{
	m_eyeTexture.clear();
		m_skyboxRenderer->setCamera(m_leftEye.get());
		m_gameObjectRenderer->setCamera(m_leftEye.get());
		m_skyboxRenderer->draw();
		m_gameObjectRenderer->draw(&m_shogiban);
		m_gameObjectRenderer->draw(&m_island);
		m_gameObjectRenderer->draw(&m_table);
	m_eyeTexture.display();

	sf::Texture::bind(&m_eyeTexture.getTexture());

	//vr::Texture_t leftEyeTexture = { (void*)(uintptr_t)eyeTexture, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Left, nullptr);

	//m_eyeTexture.clear();
	m_skyboxRenderer->setCamera(m_rightEye.get());
	m_gameObjectRenderer->setCamera(m_rightEye.get());
	m_skyboxRenderer->draw();
	m_gameObjectRenderer->draw(&m_shogiban);
	m_gameObjectRenderer->draw(&m_island);
	m_gameObjectRenderer->draw(&m_table);
	m_eyeTexture.display();

	GLuint rightTexture = m_eyeTexture.getTexture().getNativeHandle();
	//vr::Texture_t rightEyeTexture = { (void*)(uintptr_t)eyeTexture, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Right, nullptr);
}

void Game::onResize(const vec2 & windowSize)
{
	glViewport(0, 0, static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
}
