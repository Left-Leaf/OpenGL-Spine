#pragma once
#include <glad/glad.h>

enum DRAWMODE { ARRAY, ELEMENTS, ARRAY_OUTLINE,SPINE};

class Renderer {
	unsigned int ID;
	DRAWMODE drawmode;
	GLenum  mode;
	GLsizei count;
	GLenum  type;
public:
	Renderer(const DRAWMODE& drawmode, const GLenum& mode, const GLsizei& count, const GLenum& type);
	void SetID(unsigned int ID);
	void Render();
};