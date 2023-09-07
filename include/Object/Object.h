#pragma once
#include <string>
#include <glm/glm.hpp>
#include <Render/Render.h>
#include <ResourceManager/ModelManager.h>
#include <ResourceManager/ShaderManager.h>
#include <ResourceManager/TextureManager.h>
#include <ResourceManager/VertexArrayManager.h>
#include <glm/ext/matrix_transform.hpp>

/*
* 实体ID用于模板缓冲，在鼠标点击时确认当前鼠标指向的实体对象
* 模型，着色器，纹理，顶点数组用于获取渲染器，使用着色器，绑定纹理，绑定顶点数组
* 模型缩放比，位移量，欧拉角用于控制模型在世界空间中的位置
*/

class Object {
public:
	//实体ID
	unsigned int ID;
	//实体基础属性
	std::string modelName;			//模型
	std::string shaderName;			//着色器
	std::string textureName;		//纹理
	std::string vertexArrayName;	//顶点数组
	//实体空间属性
	glm::vec3 scaleVec;				//模型缩放比
	glm::vec3 translateVec;			//模型位移量(模型坐标)
	float pitch;				//俯仰角
	float yaw;				//偏航角
	float roll;				//滚转角
public:
	Object(const std::string& modelName, const std::string& shaderName, const std::string& textureName, const std::string& vertexArrayName);
	void Render();
	const void SetPosition(glm::vec3 translateVec);
	const void SetScale(glm::vec3 scaleVec);
	const void SetAngle(glm::vec3 rotateVec);
	const glm::vec3 GetPosition();
	const glm::vec3 GetScale();
	const glm::vec3 GetAngle();
};





