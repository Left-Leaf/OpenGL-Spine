#include <ResourceManager/ModelManager.h>
#include <iostream>

std::map<std::string, Model*> ModelManager::Models;
void ModelManager::Register(Model* model, std::string name)
{
	Models[name] = model;
	std::cout << "模型" << name << "已被注册" << std::endl;
}
Model* ModelManager::GetModel(std::string name)
{
	return Models[name];
}
bool ModelManager::DelModel(std::string name, bool clear)
{
	if (Models[name]) {
		Models[name]->Destory();
		delete Models[name];
		Models[name] = nullptr;
	}
	if (!clear) {
		Models.erase(name);
	}
	std::cout << "模型" << name << "已被销毁" << std::endl;
	return true;
}
void ModelManager::Clear()
{
	for (auto iter : Models)
		DelModel(iter.first, true);
	Models.clear();
}
