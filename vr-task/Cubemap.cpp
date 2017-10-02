#include "Cubemap.h"

#include <SFML/Graphics/Image.hpp>

#include "FileManager.h"

Cubemap::Cubemap()
{
	glGenTextures(1, &m_id);
}

Cubemap::~Cubemap()
{
	glDeleteTextures(1, &m_id);
}

void Cubemap::init(const std::vector<std::string>& textures)
{
	bind();

	for (size_t i = 0; i < textures.size() && i < 6; ++i) {
		std::vector<char> imageSource = FileManager::open(textures[i]);
		sf::Image image;

		if (!image.loadFromMemory(imageSource.data(), imageSource.size())) {
			throw std::runtime_error("Unable to load texture: \"" + textures[i] + "\"");
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, 
			image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	unbind();
}

void Cubemap::bind(int unit)
{
	if (unit > -1) {
		glActiveTexture(GL_TEXTURE0 + unit);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
}

void Cubemap::unbind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
