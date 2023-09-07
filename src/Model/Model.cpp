#include <Model/Model.h>
#include <ResourceManager/ShaderManager.h>
#include <ResourceManager/TextureManager.h>

const std::vector<float> OutLineModel::outlineVertices= {
	 0.5f,  1.0f, -0.0f,
	-0.5f,  1.0f, -0.0f,
	-0.5f,  0.0f, -0.0f,
	 0.5f,  0.0f, -0.0f,
};
std::string OutLineModel::vsPath = "res/shader/outline/outline.vs";
std::string OutLineModel::fsPath = "res/shader/outline/outline.fs";

void OutLineModel::setData(VertexArray* vertexArray)const
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexArray->VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, outlineVertices.size() * sizeof(float), outlineVertices.data(), GL_STATIC_DRAW);
}
void OutLineModel::setPointer(VertexArray* vertexArray)const
{
	//顶点坐标
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void OutLineModel::InitVertexArray(VertexArray* vertexArray) const
{
	vertexArray->VBO = std::vector<GLuint>(1);

	glGenVertexArrays(1, &vertexArray->VAO);
	glGenBuffers(1, &vertexArray->VBO[0]);

	glBindVertexArray(vertexArray->VAO);
	setData(vertexArray);
	setPointer(vertexArray);
	glBindVertexArray(0);
}
void OutLineModel::UpdataVertexArray(VertexArray* vertexArray) const {
	glBindVertexArray(vertexArray->VAO);
	setData(vertexArray);
	glBindVertexArray(0);
}
Renderer OutLineModel::GetRenderer() const
{
	return Renderer(DRAWMODE::ARRAY_OUTLINE, GL_LINE_LOOP, 4, GL_FLOAT);
}


const std::vector<float> PlaneModel::planeVertices = {
	 1.0f,  1.0f, -0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f, -0.0f, 0.0f, 0.0f,
	-1.0f, -1.0f, -0.0f, 0.0f, 1.0f,
	 1.0f, -1.0f, -0.0f, 1.0f, 1.0f
};
const std::vector<unsigned short> PlaneModel::planeIndices = {
	0, 1, 2,
	0, 3, 2
};
std::string PlaneModel::vsPath = "res/shader/floor/vertex.vs";
std::string PlaneModel::fsPath = "res/shader/floor/fragment.fs";

void PlaneModel::setData(VertexArray* vertexArray)const
{
	std::vector<float> vertices = planeVertices;
	vertices[3] = REPEAT_S;
	vertices[14] = REPEAT_T;
	vertices[18] = REPEAT_S;
	vertices[19] = REPEAT_T;

	glBindBuffer(GL_ARRAY_BUFFER, vertexArray->VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArray->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, planeIndices.size() * sizeof(unsigned short), planeIndices.data(), GL_STATIC_DRAW);
}
void PlaneModel::setPointer(VertexArray* vertexArray)const
{
	//顶点坐标
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//纹理坐标
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void PlaneModel::SetRepeat(float S, float T)
{
	this->REPEAT_S = S;
	this->REPEAT_T = T;
}
PlaneModel::PlaneModel(std::string path):Model() {
	this->texPath = path;
}

void PlaneModel::InitVertexArray(VertexArray* vertexArray) const
{
	vertexArray->VBO = std::vector<GLuint>(1);

	glGenVertexArrays(1, &vertexArray->VAO);
	glGenBuffers(1, &vertexArray->VBO[0]);
	glGenBuffers(1, &vertexArray->EBO);

	glBindVertexArray(vertexArray->VAO);
	setData(vertexArray);
	setPointer(vertexArray);
	glBindVertexArray(0);
}
void PlaneModel::UpdataVertexArray(VertexArray* vertexArray) const
{
	glBindVertexArray(vertexArray->VAO);
	setData(vertexArray);
	glBindVertexArray(0);
}
Renderer PlaneModel::GetRenderer() const
{
	return Renderer(DRAWMODE::ELEMENTS,GL_TRIANGLES,planeIndices.size(), GL_UNSIGNED_SHORT);
}


