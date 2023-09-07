#pragma once
#ifndef _SHADER_H
#define _SHADER_H

#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // 程序ID
    GLuint ID;

    // 构造器读取并构建着色器
    Shader() {};
    Shader& Use();
    void Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource);
    // uniform工具函数
    void SetFloat(const GLchar* name, GLfloat value, GLboolean useShader = false);
    void SetInteger(const GLchar* name, GLint value, GLboolean useShader = false);
    void SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader = false);
    void SetVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader = false);
    void SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
    void SetVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader = false);
    void SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
    void SetVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader = false);
    void SetMatrix4(const GLchar* name, const glm::mat4& matrix, GLboolean useShader = false);

private:
    void checkCompileErrors(GLuint object, std::string type);
};

#endif