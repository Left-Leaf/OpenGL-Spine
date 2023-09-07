#include <Util/Texture.h>


Texture::Texture() {}

Texture::~Texture() {}

void Texture::Bind()
{
	glBindTexture(target, ID);
}

Texture2D::Texture2D() :Texture(), Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
	target = GL_TEXTURE_2D;
}

TextureCubeMap::TextureCubeMap() :Wrap_S(GL_CLAMP_TO_EDGE), Wrap_T(GL_CLAMP_TO_EDGE), Wrap_R(GL_CLAMP_TO_EDGE), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
	for (int i = 0; i < 6; i++) {
		Width[i] = 0;
		Height[i] = 0;
		Internal_Format[i] = GL_RGB;
		Image_Format[i] = GL_RGB;
	}
	target = GL_TEXTURE_CUBE_MAP;
}


