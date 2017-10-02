#include "CubemapFactory.h"

CubemapFactory::CubemapFactory(const std::vector<std::string>& textures) :
	AbstractFactory(tag<Cubemap>{}), m_data(nullptr),
	m_textures(textures)
{
}

void * CubemapFactory::load()
{
	if (m_data == nullptr) {
		std::unique_ptr<Cubemap> cubemap = std::make_unique<Cubemap>();

		cubemap->init(m_textures);

		m_data = std::move(cubemap);
	}

	return m_data.get();
}

void CubemapFactory::clear()
{
	m_data.reset(nullptr);
}
