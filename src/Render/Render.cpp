#include <Render/Render.h>

Renderer::Renderer(const DRAWMODE& drawmode, const GLenum& mode, const GLsizei& count, const GLenum& type) :ID(0),drawmode(drawmode), mode(mode), count(count), type(type) {}

void Renderer::SetID(unsigned int ID)
{
	this->ID = ID;
}

void Renderer::Render()
{
	glStencilFunc(GL_ALWAYS, ID, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	if (drawmode == DRAWMODE::ARRAY) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawArrays(mode, 0, count);
	}
	else if (drawmode == DRAWMODE::ELEMENTS) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawElements(mode, count, type, 0);
	}
	else if (drawmode == DRAWMODE::ARRAY_OUTLINE) {
		glBlendFunc(GL_ONE, GL_ZERO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(mode, 0, count);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (drawmode == DRAWMODE::SPINE) {
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glDrawElements(mode, count, type, 0);
	}
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}
