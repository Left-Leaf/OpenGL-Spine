#pragma once
#include <Util/VertexArray.h>
#include <string>
#include <map>
#include <Model/Model.h>
#include <iostream>

class VertexArrayManager {
public:
	static std::map<std::string, VertexArray> VertexArrays;

	static VertexArray* LoadVertexArray(Model* model, std::string name);

	static VertexArray* GetVertexArray(std::string name);
	static bool DelVertexArray(std::string name,bool clear);

	static void Clear();
private:
	static VertexArray loadVertexArrayFromModel(Model* model);
};