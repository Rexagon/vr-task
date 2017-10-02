#include "ShaderFactory.h"

ShaderFactory::ShaderFactory(const std::string & vertexShader, const std::string & fragmentShader) :
	AbstractFactory(tag<Shader>{}), m_data(nullptr),
	m_vertexShader(vertexShader), m_fragmentShader(fragmentShader)
{
}

ShaderFactory::ShaderFactory(const std::string & vertexShader, const std::string & geometryShader, const std::string & fragmentShader) :
	AbstractFactory(tag<Shader>{}), m_data(nullptr),
	m_vertexShader(vertexShader), m_geometryShader(geometryShader), m_fragmentShader(fragmentShader)
{
}

void * ShaderFactory::load()
{
	if (m_data == nullptr) {
		std::unique_ptr<Shader> shader = std::make_unique<Shader>();
		if (m_shaderType == VERTEX_FRAGMENT) {
			shader->init(m_vertexShader, m_fragmentShader);
		}
		else {
			shader->init(m_vertexShader, m_geometryShader, m_fragmentShader);
		}

		m_data = std::move(shader);
	}

	return m_data.get();
}

void ShaderFactory::clear()
{
	m_data.reset(nullptr);
}
