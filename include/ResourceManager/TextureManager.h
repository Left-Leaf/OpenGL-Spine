#pragma once
#include <map>
#include <string>
#include <vector>
#include <Util/Texture.h>



class TextureManager {
public:
	static std::map<std::string, Texture*> Textures;

	static Texture2D* LoadTexture(std::string path, std::string name);
	static TextureCubeMap* LoadTexture(std::vector<std::string> faces, std::string name);
	static Texture* GetTexture(std::string name);
	static bool DelTexture(std::string name,bool clear = false);
	static void Clear();

private:
	static Texture2D* LoadTexture2DFromFile(std::string path);
	static TextureCubeMap* LoadTextureCubeMapFromFiles(std::vector<std::string> faces);
};

void load2DTex(Texture2D* texture, std::string path);
void loadCubemapTex(TextureCubeMap* texture, std::vector<std::string> faces);