#pragma once

#include <string>
#include <vector>
#include <map>

#include <GL/glew.h>

#include "Math.h"

class Shader
{
public:
	// Loads vertex and fragment shaders
	Shader();
	~Shader();

	void init(const std::string& vertexShader, const std::string& fragmentShader);
	void init(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader);

	void bind();
	void unbind();

	void setAttribute(unsigned int index, const std::string& name);

	void setUniform(const std::string& name, int data);
	void setUniform(const std::string& name, float data);
	void setUniform(const std::string& name, const vec2& data);
	void setUniform(const std::string& name, const ivec2& data);
	void setUniform(const std::string& name, const vec3& data);
	void setUniform(const std::string& name, const ivec3& data);
	void setUniform(const std::string& name, const vec4& data);
	void setUniform(const std::string& name, const ivec4& data);
	void setUniform(const std::string& name, const mat4& data);

	void setUniformArray(const std::string& name, int* data, int size);
	void setUniformArray(const std::string& name, float* data, int size);
	void setUniformArray(const std::string& name, vec2* data, int size);
	void setUniformArray(const std::string& name, ivec2* data, int size);
	void setUniformArray(const std::string& name, vec3* data, int size);
	void setUniformArray(const std::string& name, ivec3* data, int size);
	void setUniformArray(const std::string& name, vec4* data, int size);
	void setUniformArray(const std::string& name, ivec4* data, int size);
	void setUniformArray(const std::string& name, mat4* data, int size);

	unsigned int getUniformLocation(const std::string& name);

	GLuint getHandle() const { return m_id; }

private:
	void attachPart(const std::string& file, GLenum type);
	void link();

	GLuint m_id;

	std::vector<GLint> m_programs;
	std::map<std::string, GLint> m_uniformLocations;
};