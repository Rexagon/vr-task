#include "Game.h"

#include "Core.h"

void Game::onInit()
{
	initResources();

	m_skyboxRenderer = std::make_unique<SkyboxRenderer>();
	m_bodyRenderer = std::make_unique<BodyRenderer>();

	m_shogiban = m_world.createBody(ResourceManager::get<Mesh>("shogiban"), 100.0f);
	m_shogiban->setPosition(0.0f, 10.0f, 0.0f);
	m_shogiban->setDiffuseTexture(ResourceManager::get<sf::Texture>("shogiban"));
	m_shogiban->getDiffuseTexture()->setSmooth(true);

	m_island = m_world.createBody(ResourceManager::get<Mesh>("island"));
	m_island->setDiffuseTexture(ResourceManager::get<sf::Texture>("island"));
	m_island->getDiffuseTexture()->setSmooth(true);
	m_island->setPosition(0.0f, -10.0f, 0.0f);
	
	m_table = m_world.createBody(ResourceManager::get<Mesh>("table"));
	m_table->setDiffuseTexture(ResourceManager::get<sf::Texture>("table"));
	m_table->getDiffuseTexture()->setSmooth(true);

	sf::Vector2f windowSize = sf::Vector2f(Core::getWindow().getSize());

#ifdef VR_ENABLED

	m_leftEye = std::make_unique<VRCamera>(vr::Eye_Left);
	m_rightEye = std::make_unique<VRCamera>(vr::Eye_Right);

	m_leftEyeBuffer = std::make_unique<FrameBuffer>(VRsystem::getRenderTargetSize());
	m_rightEyeBuffer = std::make_unique<FrameBuffer>(VRsystem::getRenderTargetSize());

	Log::write(m_leftEyeBuffer->getColorTexture().getSize().y);

	m_leftEyeDebugSprite.setTexture(&m_leftEyeBuffer->getColorTexture(), true);
	m_rightEyeDebugSprite.setTexture(&m_rightEyeBuffer->getColorTexture(), true);

	onResize(vec2(windowSize.x, windowSize.y));

#else

	m_camera = std::make_unique<PerspectiveCamera>(glm::radians(90.0f), windowSize.x / windowSize.y, 0.1f, 1000.0f);
	m_camera->setPosition(0, 10.0f, 10.0f);

	m_skyboxRenderer->setCamera(m_camera.get());
	m_bodyRenderer->setCamera(m_camera.get());

#endif // VR_ENABLED
}

void Game::onClose()
{
	m_skyboxRenderer.reset(nullptr);
	m_bodyRenderer.reset(nullptr);
}

void Game::onUpdate(const float dt)
{
	m_world.update(dt);

	if (Input::getKeyDown(Key::Escape)) {
		SceneManager::deleteScene();
		return;
	}


#ifdef VR_ENABLED

	if (VRsystem::isHmdConnected()) {
		DeviceIndex hmdDeviceIndex = VRsystem::getHmdDeviceIndex();

		vec3 position = VRsystem::getDevicePosition(hmdDeviceIndex) * 10.0f;
		quat rotation = VRsystem::getDeviceRotation(hmdDeviceIndex);

		m_leftEye->setPosition(position);
		m_leftEye->setRotation(rotation);

		m_rightEye->setPosition(position);
		m_rightEye->setRotation(rotation);
	}

#else

	vec3 vec(0.0f, 0.0f, 0.0f);
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

	if (Input::getMouse(MouseButton::Right)) {
		vec2 mouseDelta = Input::getMouseDeltaPosition();
		if (mouseDelta != vec2()) {
			m_camera->rotate(mouseDelta.y * dt, mouseDelta.x * dt, 0.0f);
			//m_camera->rotate(-mouseDelta.y * dt, 0.0f, 0.0f);
		}
	}

	if (Input::getMouseDown(MouseButton::Left)) {
		m_figures.push_back(m_world.createBody(ResourceManager::get<Mesh>("figure"), 5.0f));
		m_figures.back()->setPosition(m_camera->getDirectionFront() + m_camera->getPosition());
		m_figures.back()->setDiffuseTexture(ResourceManager::get<sf::Texture>("figure"));
		m_figures.back()->getDiffuseTexture()->setSmooth(true);
		m_figures.back()->getPhysicsData()->activate(true);
		m_figures.back()->getPhysicsData()->applyCentralImpulse(toBT(m_camera->getDirectionFront()) * 100.0f);
	}

#endif // VR_ENABLED
}

void Game::onDraw(const float dt)
{
#ifdef VR_ENABLED
	sf::Vector2u renderTargetSize = m_leftEyeBuffer->getColorTexture().getSize();
	glViewport(0, 0, static_cast<int>(renderTargetSize.x), static_cast<int>(renderTargetSize.y));

	m_leftEyeBuffer->bind();
	m_skyboxRenderer->setCamera(m_leftEye.get());
	m_bodyRenderer->setCamera(m_leftEye.get());
	drawScene();
	m_leftEyeBuffer->unbind();

	m_rightEyeBuffer->bind();
	m_skyboxRenderer->setCamera(m_rightEye.get());
	m_bodyRenderer->setCamera(m_rightEye.get());
	drawScene();
	m_rightEyeBuffer->unbind();

	sf::Vector2u windowSize = Core::getWindow().getSize();
	glViewport(0, 0, static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));

	static vr::Texture_t leftEyeTexture = {
		(void*)(uintptr_t)m_leftEyeBuffer->getColorTexture().getNativeHandle(),
		vr::TextureType_OpenGL,
		vr::ColorSpace_Gamma
	};

	static vr::Texture_t rightEyeBuffer = {
		(void*)(uintptr_t)m_rightEyeBuffer->getColorTexture().getNativeHandle(),
		vr::TextureType_OpenGL,
		vr::ColorSpace_Gamma
	};

	vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeBuffer);

	m_leftEyeDebugSprite.setTexture(&m_leftEyeBuffer->getColorTexture());
	m_rightEyeDebugSprite.setTexture(&m_rightEyeBuffer->getColorTexture());

	Core::getWindow().pushGLStates();

	Core::getWindow().clear();
	Core::getWindow().draw(m_leftEyeDebugSprite);
	Core::getWindow().draw(m_rightEyeDebugSprite);

	Core::getWindow().popGLStates();
#else

	drawScene();

#endif // VR_ENABLED
}

void Game::onResize(const vec2 & windowSize)
{
#ifdef VR_ENABLED

	Core::getWindow().setView(sf::View(sf::FloatRect(0.f, 0.f, windowSize.x, windowSize.y)));

	sf::Vector2f halfSize = sf::Vector2f(static_cast<float>(windowSize.x) / 2.0f, static_cast<float>(windowSize.y));
	
	m_leftEyeDebugSprite.setSize(halfSize);
	m_leftEyeDebugSprite.setPosition(0.0f, 0.0f);

	m_rightEyeDebugSprite.setSize(halfSize);
	m_rightEyeDebugSprite.setPosition(halfSize.x, 0.0f);

#endif
}

void Game::initResources()
{
	ResourceManager::bind<MeshFactory>("shogiban", "models/shogiban.obj");
	ResourceManager::bind<MeshFactory>("island", "models/island.obj");
	ResourceManager::bind<MeshFactory>("table", "models/table.obj");
	ResourceManager::bind<MeshFactory>("figure", "models/figure.obj");

	ResourceManager::bind<TextureFactory>("shogiban", "textures/shogiban.png");
	ResourceManager::bind<TextureFactory>("island", "textures/island.png");
	ResourceManager::bind<TextureFactory>("table", "textures/table.png");

	ResourceManager::bind<TextureFactory>("figure", "textures/figure_kingw.png");
	ResourceManager::bind<TextureFactory>("figure", "textures/figure_kingb.png");
	ResourceManager::bind<TextureFactory>("figure", "textures/figure_gold.png");
	ResourceManager::bind<TextureFactory>("figure", "textures/figure_silver.png");
	ResourceManager::bind<TextureFactory>("figure", "textures/figure_knight.png");
	ResourceManager::bind<TextureFactory>("figure", "textures/figure_lance.png");
	ResourceManager::bind<TextureFactory>("figure", "textures/figure_rook.png");
	ResourceManager::bind<TextureFactory>("figure", "textures/figure_bishop.png");
	ResourceManager::bind<TextureFactory>("figure", "textures/figure_pawn.png");
}

void Game::initBoard()
{
	enum FigureType {
		KINGB,
		KINGW,
		GOLD,
		SILVER,
		KNIGHT,
		LANCE,
		ROOK,
		BISHOP,
		PAWN
	};


}

void Game::drawScene()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_skyboxRenderer->draw();
	m_bodyRenderer->draw(m_shogiban);
	m_bodyRenderer->draw(m_island);
	m_bodyRenderer->draw(m_table);

	for (auto& test : m_figures) {
		m_bodyRenderer->draw(test);
	}
}
