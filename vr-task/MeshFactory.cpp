#include "MeshFactory.h"

#include <algorithm>
#include <map>

#include "FileManager.h"
#include "Stuff.h"

MeshFactory::MeshFactory(const std::string & filename) :
	AbstractFactory(tag<Mesh>{}), m_data(nullptr),
	m_filename(filename)
{
}

void * MeshFactory::load()
{
	if (m_data == nullptr) {
		loadMesh();
	}

	return m_data.get();
}

void MeshFactory::clear()
{
	m_data.reset(nullptr);
}

void MeshFactory::loadMesh()
{
	struct Vertex {
		vec3 position;
		vec2 textureCoords;
		vec3 normal;
	};

	bool hasPositions = false;
	bool hasTextureCoords = false;
	bool hasNormals = false;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	{
		std::vector<char> file = FileManager::open(m_filename);

		std::vector<vec3> positions;
		std::vector<vec2> textureCoords;
		std::vector<vec3> normals;
		std::map<std::string, unsigned int> verticesToIndices;

		int lineBeginning = 0;
		for (size_t lineEnding = 0; lineEnding < file.size(); ++lineEnding) {
			if (file[lineEnding] != '\n' && lineEnding + 1 != file.size()) {
				continue;
			}

			std::string line;
			line.resize(lineEnding - lineBeginning);
			memcpy(&line[0], &file[0] + lineBeginning, line.size());
			lineBeginning = static_cast<int>(lineEnding) + 1;

			std::vector<std::string> tokens = split(line, ' ');
			if (tokens.size() > 1) {
				if (tokens[0] == "v" && tokens.size() == 4) {
					// Parse position
					vec3 position;
					for (size_t i = 0; i < 3; ++i) {
						position[i] = std::stof(tokens[i + 1]);
					}
					positions.push_back(position);
				}
				else if (tokens[0] == "vt" && tokens.size() == 3) {
					// Parse texture coords
					vec2 textureCoord;
					for (size_t i = 0; i < 2; ++i) {
						textureCoord[i] = std::stof(tokens[i + 1]);
					}
					textureCoord.y = 1.0f - textureCoord.y;
					textureCoords.push_back(textureCoord);
				}
				else if (tokens[0] == "vn" && tokens.size() == 4) {
					// Parse normal
					vec3 normal;
					for (size_t i = 0; i < 3; ++i) {
						normal[i] = std::stof(tokens[i + 1]);
					}
					normals.push_back(normal);
				}
				else if (tokens[0] == "f" && tokens.size() == 4) {
					// Parse face
					for (size_t i = 0; i < 3; ++i) {
						const std::string& token = tokens[i + 1];
						auto it = verticesToIndices.find(token);
						if (it == verticesToIndices.end()) {
							unsigned int index = static_cast<unsigned int>(vertices.size());

							Vertex vertex;

							std::vector<std::string> subtokens = split(token, '/');
							if (subtokens.size() > 0) {
								vertex.position = positions.at(std::stoull(subtokens[0]) - 1);
							}
							if (subtokens.size() > 1) {
								vertex.textureCoords = textureCoords.at(std::stoull(subtokens[1]) - 1);
							}
							if (subtokens.size() > 2) {
								vertex.normal = normals.at(std::stoull(subtokens[2]) - 1);
							}

							vertices.push_back(vertex);

							verticesToIndices[token] = index;
							indices.push_back(index);
						}
						else {
							indices.push_back(it->second);
						}
					}
				}
			}
		}

		hasPositions = !positions.empty();
		hasTextureCoords = !textureCoords.empty();
		hasNormals = !normals.empty();
	}

	std::vector<vec3> positions;
	std::vector<vec2> textureCoords;
	std::vector<vec3> normals;

	for (size_t i = 0; i < vertices.size(); ++i) {
		if (hasPositions) {
			if (i == 0) {
				positions.resize(vertices.size());
			}
			positions[i] = vertices[i].position;
		}

		if (hasTextureCoords) {
			if (i == 0) {
				textureCoords.resize(vertices.size());
			}
			textureCoords[i] = vertices[i].textureCoords;
		}

		if (hasNormals) {
			if (i == 0) {
				normals.resize(vertices.size());
			}
			normals[i] = vertices[i].normal;
		}
	}

	m_data = std::make_unique<Mesh>();

	if (hasPositions && !hasTextureCoords && !hasNormals) {
		m_data->init(positions, indices);
	}
	else if (hasPositions && hasTextureCoords && !hasNormals) {
		m_data->init(positions, textureCoords, indices);
	}
	else if (hasPositions && hasTextureCoords && hasNormals) {
		m_data->init(positions, textureCoords, normals, indices);
	}
}
