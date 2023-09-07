#pragma once
#include <Model/Model.h>
#include <glm/glm.hpp>
namespace mesh {
	struct Vertex {
		float x, y, z;
	};
}

glm::vec3 lastPosition(0.0f);

class MeshModel:public Model {
private:
	int size;//网格大小
	int offset = 1;
	std::vector<mesh::Vertex> meshVertices;
public:
	static std::string vsPath;
	static std::string fsPath;
	MeshModel(int size,int offset);
	void UpdataMesh(glm::vec3 position);
	glm::vec3 GetOffset(glm::vec3 position);
	//重写基类函数
	void InitVertexArray(VertexArray* vertexArray) const override;
	void UpdataVertexArray(VertexArray* vertexArray) const override;
	Renderer GetRenderer() const override;
	void Destory() override {};
};

std::string MeshModel::vsPath = "res/shader/mesh/mesh.vs";
std::string MeshModel::fsPath = "res/shader/mesh/mesh.fs";

inline MeshModel::MeshModel(int size, int offset)
{
	this->size = size;
	this->offset = offset;
}
inline void MeshModel::UpdataMesh(glm::vec3 position) {
	meshVertices.clear();
	int x = position.x;
	int z = position.z;
	x = x - x % offset;
	z = z - z % offset;
	//y轴线
	for (int i = -size; i <= size; i++) {
		mesh::Vertex v1 = { x + offset * i, 0, z + offset * size };
		mesh::Vertex v2 = { x + offset * i, 0, z - offset * size };
		meshVertices.push_back(v1);
		meshVertices.push_back(v2);
	}
	//x轴线
	for (int i = -size; i <= size; i++) {
		mesh::Vertex v1 = { x + offset * size, 0, z + offset * i  };
		mesh::Vertex v2 = { x - offset * size, 0, z + offset * i  };
		meshVertices.push_back(v1);
		meshVertices.push_back(v2);
	}
	lastPosition = position;
}
inline glm::vec3 MeshModel::GetOffset(glm::vec3 position)
{
	int x_ = position.x - lastPosition.x;
	int z_ = position.z - lastPosition.z;
	if ((abs(x_) + abs(z_)) < offset) {
		return glm::vec3(0.0f);
	}
	int x = round(position.x);
	int z = round(position.z);
	x = x - x % offset;
	z = z - z % offset;
	return glm::vec3(x, 0, z);
}

inline void MeshModel::InitVertexArray(VertexArray* vertexArray) const
{
	vertexArray->VBO = std::vector<GLuint>(1);

	glGenVertexArrays(1, &vertexArray->VAO);
	glGenBuffers(1, &vertexArray->VBO[0]);

	glBindVertexArray(vertexArray->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArray->VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(mesh::Vertex), meshVertices.data(), GL_STATIC_DRAW);
	//顶点坐标
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
inline void MeshModel::UpdataVertexArray(VertexArray* vertexArray) const
{
	glBindVertexArray(vertexArray->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArray->VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(mesh::Vertex), meshVertices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}
inline Renderer MeshModel::GetRenderer() const
{
	return Renderer(DRAWMODE::ARRAY, GL_LINES, meshVertices.size(), GL_FLOAT);
}
