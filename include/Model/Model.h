#pragma once
#include <vector>
#include <string>
#include <glad/glad.h>
#include <Util/VertexArray.h>
#include <Render/Render.h>

class Model {
public:
	virtual void InitVertexArray(VertexArray* vertexArray) const = 0;//初始化顶点数组
	virtual void UpdataVertexArray(VertexArray* vertexArray) const = 0;//更新顶点数组
	virtual Renderer GetRenderer() const = 0;
	virtual void Destory() = 0;
};

class OutLineModel :public Model {
private:
	static const std::vector<float> outlineVertices;
	void setData(VertexArray* vertexArray) const;
	void setPointer(VertexArray* vertexArray) const;
public:
	static std::string vsPath;
	static std::string fsPath;
	//重写基类函数
	void InitVertexArray(VertexArray* vertexArray) const override;
	void UpdataVertexArray(VertexArray* vertexArray) const override;
	Renderer GetRenderer() const override;
	void Destory() override {};
};

class PlaneModel :public Model {
private:
	static const std::vector<float> planeVertices;
	static const std::vector<unsigned short> planeIndices;
	void setData(VertexArray* vertexArray) const;
	void setPointer(VertexArray* vertexArray) const;
	float REPEAT_S = 1.0f;
	float REPEAT_T = 1.0f;
public:
	std::string texPath;
	static std::string vsPath;
	static std::string fsPath;
	PlaneModel(std::string path);
	void SetRepeat(float S, float T);
	//重写基类函数
	void InitVertexArray(VertexArray* vertexArray) const override;
	void UpdataVertexArray(VertexArray* vertexArray) const override;
	Renderer GetRenderer() const override;
	void Destory() override {};
};



