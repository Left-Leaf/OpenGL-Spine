#include <Model/SpineModel.h>
#include <ResourceManager/TextureManager.h>
#include <ResourceManager/ShaderManager.h>

spine::SpineExtension* spine::getDefaultExtension() {
    return new spine::DefaultSpineExtension();
}

MYTextureLoader::MYTextureLoader(const std::string& name)
    : name(name)
{
}
void MYTextureLoader::load(spine::AtlasPage& page, const spine::String& path)
{
	Texture2D* texture = TextureManager::LoadTexture(path.buffer(), name);

    GLenum minFilter = 0;
    if (page.minFilter == spine::TextureFilter_Linear)
        minFilter = GL_LINEAR;
    else if (page.minFilter == spine::TextureFilter_Nearest)
        minFilter = GL_NEAREST;
    else if (page.minFilter == spine::TextureFilter_MipMapLinearLinear)
        minFilter = GL_LINEAR_MIPMAP_LINEAR;
    else if (page.minFilter == spine::TextureFilter_MipMapLinearLinear)
        minFilter = GL_LINEAR_MIPMAP_LINEAR;
    else if (page.minFilter == spine::TextureFilter_MipMapLinearNearest)
        minFilter = GL_LINEAR_MIPMAP_NEAREST;
    else if (page.minFilter == spine::TextureFilter_MipMapNearestNearest)
        minFilter = GL_NEAREST_MIPMAP_NEAREST;
    else if (page.minFilter == spine::TextureFilter_MipMapNearestLinear)
        minFilter = GL_NEAREST_MIPMAP_LINEAR;

    GLenum magFilter = 0;
    if (page.magFilter == spine::TextureFilter_Linear)
        magFilter = GL_LINEAR;
    else if (page.magFilter == spine::TextureFilter_Nearest)
        magFilter = GL_NEAREST;
    else if (page.magFilter == spine::TextureFilter_MipMapLinearLinear)
        magFilter = GL_LINEAR_MIPMAP_LINEAR;
    else if (page.magFilter == spine::TextureFilter_MipMapLinearLinear)
        magFilter = GL_LINEAR_MIPMAP_LINEAR;
    else if (page.magFilter == spine::TextureFilter_MipMapLinearNearest)
        magFilter = GL_LINEAR_MIPMAP_NEAREST;
    else if (page.magFilter == spine::TextureFilter_MipMapNearestNearest)
        magFilter = GL_NEAREST_MIPMAP_NEAREST;
    else if (page.magFilter == spine::TextureFilter_MipMapNearestLinear)
        magFilter = GL_NEAREST_MIPMAP_LINEAR;

    texture->Wrap_S = GL_CLAMP_TO_EDGE;
    texture->Wrap_T = GL_CLAMP_TO_EDGE;
    texture->Filter_Min = minFilter;
    texture->Filter_Max = magFilter;

    texture->Bind();
    glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, texture->Filter_Min);
    glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, texture->Filter_Max);
    glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, texture->Wrap_S);
    glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, texture->Wrap_T);
    glBindTexture(texture->target, 0);

	page.setRendererObject(texture);
	page.width = texture->Width;
	page.height = texture->Height;
}
void MYTextureLoader::unload(void* texture)
{
	if (!texture)return;
	TextureManager::DelTexture(name);
	texture = nullptr;
}

const std::string SpineModel::vsPath = "res/shader/spine/spineModel.vs";
const std::string SpineModel::fsPath = "res/shader/spine/spineModel.fs";

SpineModel::SpineModel() :Model() {}
SpineModel::SpineModel(const std::string& name, const char* atlasPath, const char* dataPath, DataMode mode):Model()
{
    atlas = new spine::Atlas(atlasPath, new MYTextureLoader(name));
    if (atlas->getPages().size() == 0) {
        printf("Failed to load atlas");
        delete atlas;
        return;
    }
    if (mode == DataMode::JSON) {
        spine::SkeletonJson json(atlas);
        skeletonData = json.readSkeletonDataFile(dataPath);
    }
    else if (mode == DataMode::BINARY) {
        spine::SkeletonBinary binary(atlas);
        skeletonData = binary.readSkeletonDataFile(dataPath);
    }
    if (!skeletonData) {
        printf("Failed to load skeleton data");
        delete atlas;
        return;
    }
    animationStateData = new spine::AnimationStateData(skeletonData);
    animationStateData->setDefaultMix(0.5f);

    skeleton = new spine::Skeleton(skeletonData);
    animationState = new spine::AnimationState(animationStateData);

}

void SpineModel::InitVertexArray(VertexArray* vertexArray) const
{
	vertexArray->VBO = std::vector<GLuint>(1);

	glGenVertexArrays(1, &vertexArray->VAO);
	glGenBuffers(1, &vertexArray->VBO[0]);
	glGenBuffers(1, &vertexArray->EBO);

	glBindVertexArray(vertexArray->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArray->VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(spine::Vertex), vertices.data(), GL_STREAM_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArray->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STREAM_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(spine::Vertex), (void*)0);
	glEnableVertexAttribArray(0);					
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(spine::Vertex), (void*)offsetof(spine::Vertex,r));
	glEnableVertexAttribArray(1);					
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(spine::Vertex), (void*)offsetof(spine::Vertex, u));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}
void SpineModel::UpdataVertexArray(VertexArray* vertexArray) const
{
    glBindVertexArray(vertexArray->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, vertexArray->VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(spine::Vertex), vertices.data(), GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArray->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STREAM_DRAW);

    glBindVertexArray(0);
}
Renderer SpineModel::GetRenderer() const
{
    return Renderer(DRAWMODE::SPINE, GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT);
}
void SpineModel::Destory()
{
    delete atlas;
    delete skeletonData;
    delete animationStateData;
    delete skeleton;
    delete animationState;
}

void SpineModel::SetState(std::string animationName, float x, float y, size_t index)
{
    skeleton->setX(x);
    skeleton->setY(y);
    animationState->setAnimation(index, animationName.c_str(), true);
}
Texture* SpineModel::GetTexture()
{
    return (Texture*)this->atlas->getPages()[0]->getRendererObject();
}
void SpineModel::UpdataMesh(float time)
{
    //清空顶点数据和索引数据
    std::vector<spine::Vertex>().swap(vertices);
    std::vector<unsigned short>().swap(indices);

    // 更新动画状态
    animationState->update(time);
    // 更新骨骼数据
    animationState->apply(*skeleton);
    // 转换到世界坐标系
    skeleton->updateWorldTransform();
    for (size_t i = 0, n = skeleton->getSlots().size(); i < n; ++i) {
        spine::Slot* slot = skeleton->getDrawOrder()[i];
        spine::Attachment* attachment = slot->getAttachment();
        if (!attachment) {
            continue;
        }
        spine::Vertex vertex;
        vertex.z = 0.1 * i;
        std::vector<spine::Vertex> _vertices;
        std::vector<unsigned short> _indices;

        if (attachment->getRTTI().isExactly(spine::RegionAttachment::rtti)) {
            spine::RegionAttachment* regionAttachment = (spine::RegionAttachment*)attachment;
            //顶点数组
            _vertices = std::vector<spine::Vertex>(4, vertex);
            regionAttachment->computeWorldVertices(slot->getBone(), (float*)&_vertices[0], 0, sizeof(spine::Vertex) / sizeof(float));
            for (size_t j = 0, l = 0; j < _vertices.size(); j++, l += 2) {
                _vertices[j].u = regionAttachment->getUVs()[l];
                _vertices[j].v = regionAttachment->getUVs()[l + 1];
            }
            //数组索引
            unsigned short size = vertices.size();
            _indices = {size, (unsigned short)(size + 1), (unsigned short)(size + 2), (unsigned short)(size + 2), (unsigned short)(size + 3), size};
        }
        else if (attachment->getRTTI().isExactly(spine::MeshAttachment::rtti)) {
            spine::MeshAttachment* meshAttachment = (spine::MeshAttachment*)attachment;
            //顶点数组
            size_t numVertices = meshAttachment->getWorldVerticesLength() / 2;
            _vertices = std::vector<spine::Vertex>(numVertices, vertex);
            meshAttachment->computeWorldVertices(*slot, 0, 2 * numVertices, (float*)&_vertices[0], 0, sizeof(spine::Vertex) / sizeof(float));
            for (size_t j = 0, l = 0; j < _vertices.size(); j++, l += 2) {
                _vertices[j].u = meshAttachment->getUVs()[l];
                _vertices[j].v = meshAttachment->getUVs()[l + 1];
            }
            //数组索引
            size_t numIndices = meshAttachment->getTriangles().size();
            _indices = std::vector<unsigned short>(numIndices);
            unsigned short* triangles = meshAttachment->getTriangles().buffer();
            unsigned short size = vertices.size();
            for (int j = 0; j < numIndices; j++) {
                _indices[j] = triangles[j]+size;
            }
        }
        vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());
        indices.insert(indices.end(), _indices.begin(), _indices.end());
    }
}






