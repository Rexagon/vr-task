#pragma once

#include <memory>

#include "AbstractFactory.h"
#include "Cubemap.h"

class CubemapFactory : public AbstractFactory
{
public:
	CubemapFactory(const std::vector<std::string>& textures);

	void* load() override;
	void clear() override;

private:
	std::vector<std::string> m_textures;

	std::unique_ptr<Cubemap> m_data;
};