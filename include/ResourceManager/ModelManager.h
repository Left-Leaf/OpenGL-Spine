#pragma once
#include <map>
#include <string>
#include <vector>
#include <Model/Model.h>

class ModelManager
{
public:
	static std::map<std::string, Model*> Models;

	static void Register(Model* model, std::string name);
	static Model* GetModel(std::string name);
	static bool DelModel(std::string name, bool clear = false);
	static void Clear();
};

