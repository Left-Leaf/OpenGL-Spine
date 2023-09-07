#ifndef SPINEMODEL_H
#define SPINEMODEL_H

#include <spine/TextureLoader.h>
#include <spine/Atlas.h>
#include <spine/Skeleton.h>
#include <spine/SkeletonJson.h>
#include <spine/SkeletonBinary.h>
#include <spine/SkeletonData.h>
#include <spine/AnimationStateData.h>
#include <spine/AnimationState.h>
#include <spine/Attachment.h>
#include <spine/RegionAttachment.h>
#include <spine/MeshAttachment.h>

#include <Model/Model.h>
#include <Util/Texture.h>

enum DataMode { JSON, BINARY };

spine::SpineExtension* spine::getDefaultExtension();

class MYTextureLoader : public spine::TextureLoader {
private:
	std::string name;
public:
	MYTextureLoader(const std::string& name);
    void load(spine::AtlasPage& page, const spine::String& path);
    void unload(void* texture);
};

namespace spine {
	struct Vertex {
		float x, y, z;
		float r, g, b, a;
		float u, v;
	};
}

class SpineModel:public Model
{
private:
	spine::Atlas* atlas = NULL;
	spine::SkeletonData* skeletonData = NULL;
	spine::AnimationStateData* animationStateData = NULL;
	spine::Skeleton* skeleton = NULL;
	spine::AnimationState* animationState = NULL;
	
	std::vector<spine::Vertex> vertices;
	std::vector<unsigned short> indices;
public:
	static const std::string vsPath;
	static const std::string fsPath;

	SpineModel();
	SpineModel(const std::string& name, const char* atlasPath, const char* dataPath, DataMode mode);

	void InitVertexArray(VertexArray* vertexArray) const override;
	void UpdataVertexArray(VertexArray* vertexArray)const override;
	Renderer GetRenderer() const override;;
	void Destory() override;

	void SetState(std::string animationName, float x, float y, size_t index);
	Texture* GetTexture();
	void UpdataMesh(float time);
};

#endif