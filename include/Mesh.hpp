#pragma once
#include <Utils.hpp>
#include <iostream>
#include <vector>
#include <Shader.hpp>

class Mesh
{
public:
	typedef std::vector<Vertex> VertexVector;
	typedef std::vector<unsigned int> IndexVector;
	typedef std::vector<Texture> TextureVector;


	VertexVector vertices;
	IndexVector indices;
	TextureVector textures;

	Mesh(const VertexVector& vertices, const IndexVector& indices, const TextureVector& textures);

	void Draw(Shader& shader);

private:
	void _setupMesh();

	unsigned int mVBO;
	unsigned int mVAO;
	unsigned int mEBO;
};

