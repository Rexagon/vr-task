#pragma once

#include <memory>

#include "AbstractFactory.h"
#include "Shader.h"

class ShaderFactory : public AbstractFactory
{
public:
	ShaderFactory(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderFactory(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader);

	void* load() override;
	void clear() override;

private:
	enum ShaderType {
		VERTEX_FRAGMENT,
		VERTEX_GEOMETRY_FRAGMENT
	} m_shaderType;

	std::string m_vertexShader;
	std::string m_fragmentShader;
	std::string m_geometryShader;

	std::unique_ptr<Shader> m_data;
};