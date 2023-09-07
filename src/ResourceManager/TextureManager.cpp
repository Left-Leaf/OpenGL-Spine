#include <ResourceManager/TextureManager.h>
#include <stb_image.h>
#include <iostream>


std::map<std::string, Texture*> TextureManager::Textures;

Texture2D* TextureManager::LoadTexture(std::string path, std::string name)
{
	Texture2D* texture = LoadTexture2DFromFile(path);
	Textures[name] = texture;
	std::cout << "2D纹理" << name << "已被创建" << std::endl;
	return dynamic_cast<Texture2D*>(Textures[name]);
}
TextureCubeMap* TextureManager::LoadTexture(std::vector<std::string> faces, std::string name)
{
	TextureCubeMap* texture = LoadTextureCubeMapFromFiles(faces);
	Textures[name] = texture;
	std::cout << "方块纹理" << name << "已被创建" << std::endl;
	return dynamic_cast<TextureCubeMap*>(Textures[name]);
}

Texture* TextureManager::GetTexture(std::string name)
{
	return Textures[name];
}
bool TextureManager::DelTexture(std::string name, bool clear)
{
	if (Textures[name]) {
		glDeleteTextures(1, &Textures[name]->ID);
		delete Textures[name];
	}
	Textures[name] = nullptr;
	if (!clear) {
		Textures.erase(name);
	}
	std::cout << "纹理" << name << "已被销毁" << std::endl;
	return true;
}
void TextureManager::Clear()
{
	for (auto iter : Textures)
		DelTexture(iter.first, true);
	Textures.clear();
}

Texture2D* TextureManager::LoadTexture2DFromFile(std::string path)
{
	Texture2D* tex = new Texture2D();
	load2DTex(tex, path);
	return tex;
}
TextureCubeMap* TextureManager::LoadTextureCubeMapFromFiles(std::vector<std::string> faces)
{
	TextureCubeMap* tex = new TextureCubeMap();
	loadCubemapTex(tex, faces);
	return tex;
}

void load2DTex(Texture2D* texture, std::string path) {

	glGenTextures(1, &texture->ID);
	texture->Bind();

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		texture->Width = width, texture->Height = height, texture->Internal_Format = format, texture->Image_Format = format;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, texture->Filter_Min);
		glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, texture->Filter_Max);
		glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, texture->Wrap_S);
		glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, texture->Wrap_T);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	glBindTexture(texture->target, 0);
}
void loadCubemapTex(TextureCubeMap* texture, std::vector<std::string> faces)
{
	if (faces.size() != 6) {
		std::cout << "纹理数据不足" << std::endl;
		return;
	}
	glGenTextures(1, &texture->ID);
	texture->Bind();

	int width, height, nrChannels;
	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			GLenum format;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 4)
				format = GL_RGBA;

			texture->Width[i] = width, texture->Height[i] = height, texture->Internal_Format[i] = format, texture->Image_Format[i] = format;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, texture->Filter_Min);
	glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, texture->Filter_Max);
	glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, texture->Wrap_S);
	glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, texture->Wrap_T);
	glTexParameteri(texture->target, GL_TEXTURE_WRAP_R, texture->Wrap_R);

	glBindTexture(texture->target, 0);
}