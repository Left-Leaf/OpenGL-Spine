#include <Object/Object.h>

Object::Object(const std::string& modelName, const std::string& shaderName, const std::string& textureName, const std::string& vertexArrayName)
	: modelName(modelName), shaderName(shaderName), textureName(textureName), vertexArrayName(vertexArrayName), ID(0), scaleVec(glm::vec3(1.0f)), translateVec(glm::vec3(0.0f)), pitch(0.0f), yaw(0.0f), roll(0.0f) {

}

void Object::Render() {
	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, translateVec);
	model = glm::scale(model, scaleVec);

	ShaderManager::GetShader(shaderName)->Use();
	ShaderManager::GetShader(shaderName)->SetMatrix4("model", model);
	TextureManager::GetTexture(textureName)->Bind();
	VertexArrayManager::GetVertexArray(vertexArrayName)->Bind();
	Renderer renderer = ModelManager::GetModel(modelName)->GetRenderer();
	renderer.SetID(ID);
	renderer.Render();
	VertexArrayManager::GetVertexArray(vertexArrayName)->unBind();
}

const void Object::SetPosition(glm::vec3 translateVec)
{
	this->translateVec = translateVec;
}
const void Object::SetScale(glm::vec3 scaleVec)
{
	this->scaleVec = scaleVec;
}
const void Object::SetAngle(glm::vec3 rotateVec)
{
	this->pitch = rotateVec.x;
	this->yaw = rotateVec.y; 
	this->roll = rotateVec.z;
}



const glm::vec3 Object::GetPosition()
{
	return translateVec;
}
const glm::vec3 Object::GetScale()
{
	return scaleVec;
}
const glm::vec3 Object::GetAngle()
{
	return glm::vec3(pitch, yaw, roll);
}



