#pragma once

#include <memory>

#include "AbstractFactory.h"
#include "Mesh.h"

class MeshFactory : public AbstractFactory
{
public:
	MeshFactory(const std::string& filename);

	void* load() override;
	void clear() override;

private:
	void loadMesh();

	std::string m_filename;
	std::unique_ptr<Mesh> m_data;
};