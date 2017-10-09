#include "Core.h"

#include "ResourceManager.h"
#include "SceneManager.h"
#include "FileManager.h"
#include "Input.h"

#include "Log.h"

bool Core::m_isInitialized = false;
bool Core::m_isRunning = false;
sf::RenderWindow Core::m_window;

void Core::init()
{
	Log::init();
	
	ResourceManager::init();
	FileManager::init<DefaultFileSystem>();

#ifdef VR_ENABLED
	VRsystem::init();
#endif // VR_ENABLED

	sf::ContextSettings settings;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.antialiasingLevel = 4;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	m_window.create(sf::VideoMode(1024, 768), "VR task", sf::Style::Default, settings);
	m_window.setVerticalSyncEnabled(true);
	m_window.setActive(true);

	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Unable to initialize glew");
	}

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	m_isInitialized = true;
}

void Core::close()
{
	if (m_isInitialized == false || m_isRunning == true) {
		return;
	}

	SceneManager::close();
	ResourceManager::close();
	FileManager::close();

#ifdef VR_ENABLED
	VRsystem::close();
#endif // VR_ENABLED

	m_window.close();

	m_isRunning = false;
	m_isInitialized = false;
}

void Core::run()
{
	if (m_isInitialized == false || m_isRunning == true) {
		return;
	}

	m_isRunning = true;

	sf::Clock timer;

	while (m_isRunning) {
		float dt = timer.restart().asSeconds();

		// Handle window and keyboard events
		handleEvents();

#ifdef VR_ENABLED
		VRsystem::handleEvents();
#endif // VR_ENABLED

		Scene* currentScene;

		if (m_isRunning == true &&
			(currentScene = SceneManager::getCurrentScene()) != nullptr) 
		{
			currentScene->onUpdate(dt);
		}

		if (m_isRunning == true && 
			(currentScene = SceneManager::getCurrentScene()) != nullptr) 
		{
			currentScene->onDraw(dt);
		}

		m_window.display();

		m_isRunning &= SceneManager::getCurrentScene() != nullptr;
	}
}

void Core::stop()
{
	m_isRunning = false;
}

sf::RenderWindow& Core::getWindow()
{
	return m_window;
}

void Core::handleEvents()
{
	Input::reset();

	sf::Event e;
	while (m_window.pollEvent(e)) {
		switch (e.type) {
		case sf::Event::Closed:
			stop();
			break;
			
		case sf::Event::MouseMoved:
			Input::m_currentCursorPosition.x = e.mouseMove.x;
			Input::m_currentCursorPosition.y = e.mouseMove.y;
			break;

		case sf::Event::KeyPressed:
			Input::m_currentKeyStates.set(e.key.code, true);
			break;

		case sf::Event::KeyReleased:
			Input::m_currentKeyStates.set(e.key.code, false);
			break;

		case sf::Event::MouseButtonPressed:
			Input::m_currentMouseStates.set(e.mouseButton.button, true);
			break;

		case sf::Event::MouseButtonReleased:
			Input::m_currentMouseStates.set(e.mouseButton.button, false);
			break;

		case sf::Event::Resized:
			if (SceneManager::hasScenes()) {
				SceneManager::getCurrentScene()->onResize(vec2(
					static_cast<float>(e.size.width),
					static_cast<float>(e.size.height)
				));
			}
			break;

		case sf::Event::GainedFocus:
			if (SceneManager::hasScenes()) {
				SceneManager::getCurrentScene()->onFocusGained();
			}
			break;

		case sf::Event::LostFocus:
			if (SceneManager::hasScenes()) {
				SceneManager::getCurrentScene()->onFocusLost();
			}
			break;

		default:
			break;
		}
	}
}
