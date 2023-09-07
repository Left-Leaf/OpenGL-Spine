#pragma once
#include <glad/glad.h>
#include <vector>

class VertexArray {
public:
	GLuint VAO;//顶点数组索引
	std::vector<GLuint> VBO;//顶点缓冲索引
	GLuint EBO;//数组缓冲索引
	VertexArray();
	void Bind();
	void unBind();
};

