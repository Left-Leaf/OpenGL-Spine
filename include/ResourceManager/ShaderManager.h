#pragma once

#include <map>
#include <Util/Shader.h>

class ShaderManager {
public:
	static std::map<std::string, Shader> Shaders;

	static Shader* LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);

	static Shader* GetShader(std::string name);
	static bool DelShader(std::string name, bool clear = false);

	static void Clear();

private:
	static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
};
