#include <Util/VertexArray.h>

VertexArray::VertexArray(){}

void VertexArray::Bind()
{
	glBindVertexArray(VAO);
}

void VertexArray::unBind()
{
	glBindVertexArray(0);
}

