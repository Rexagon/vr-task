#pragma once

#include <SFML/Graphics.hpp>
#include <GL/glew.h>

#include "ResourceManager.h"
#include "SceneManager.h"
#include "FileManager.h"

#include "TextureFactory.h"
#include "CubemapFactory.h"
#include "ShaderFactory.h"
#include "MeshFactory.h"

#include "GameObjectRenderer.h"
#include "SkyboxRenderer.h"

#include "World.h"

#include "Input.h"
#include "Log.h"

class Core
{
public:
	// Creates window, initializes all managers
	static void init();

	// Clears up all data
	// DON'T USE IT FOR EXITING FROM GAME, USE stop() INSTED
	static void close();

	// Starts main game loop
	static void run();

	// Stops game loop
	static void stop();

	static sf::RenderWindow& getWindow();

private:
	static void handleEvents();

	static bool m_isInitialized;
	static bool m_isRunning;

	static sf::RenderWindow m_window;
};