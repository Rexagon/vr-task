#include "Core.h"

#include "Game.h"

int main() {
	Core::init();

	try {
		SceneManager::addScene<Game>();

		Core::run();
	}
	catch (const std::exception& e) {
		Log::write("Error:", e.what());
	}

	Core::close();

	return 0;
}