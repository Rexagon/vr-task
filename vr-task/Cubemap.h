#pragma once

#include <vector>

#include <GL/glew.h>

class Cubemap
{
public:
	Cubemap();
	~Cubemap();

	// Initializes cubemap from vector of textures
	// pos x, neg x, pos y, neg y, pos z, neg z
	void init(const std::vector<std::string>& textures);

	void bind(int unit = -1);
	void unbind();

	GLuint getHandle() const { return m_id; }

private:
	GLuint m_id;
};