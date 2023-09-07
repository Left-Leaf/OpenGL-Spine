#include <ResourceManager/VertexArrayManager.h>

std::map<std::string, VertexArray> VertexArrayManager::VertexArrays;

VertexArray* VertexArrayManager::LoadVertexArray(Model* model, std::string name)
{
    VertexArrays[name] = loadVertexArrayFromModel(model);
    std::cout << "顶点数组" << name << "已被创建" << std::endl;
    return &VertexArrays[name];
}

VertexArray* VertexArrayManager::GetVertexArray(std::string name)
{
    return &VertexArrays[name];
}
bool VertexArrayManager::DelVertexArray(std::string name, bool clear)
{
    glDeleteVertexArrays(1, &VertexArrays[name].VAO);
    for (auto iter : VertexArrays[name].VBO) {
        glDeleteBuffers(1, &iter);
    }
    glDeleteBuffers(1, &VertexArrays[name].EBO);
    if (!clear) {
        VertexArrays.erase(name);
    }
    std::cout << "顶点数组" << name << "已被销毁" << std::endl;
    return true;
}
void VertexArrayManager::Clear()
{
    for (auto iter : VertexArrays)
        DelVertexArray(iter.first, true);
    VertexArrays.clear();
}

VertexArray VertexArrayManager::loadVertexArrayFromModel(Model* model)
{
    //创建顶点数组
    VertexArray vertexArray;
    //初始化顶点数组
    model->InitVertexArray(&vertexArray);
    //返回顶点数组
    return vertexArray;
}
