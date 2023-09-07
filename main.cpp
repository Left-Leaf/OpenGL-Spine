#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image.h>

#include <GLContext.h>
#include <Model/SpineModel.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <ResourceManager/ShaderManager.h>
#include <ResourceManager/TextureManager.h>
#include <ResourceManager/VertexArrayManager.h>
#include <ResourceManager/ModelManager.h>
#include <Model/MeshModel.h>
#include <Camera.h>

// window
GLContext context;
int SCR_WIDTH = 1600;
int SCR_HEIGHT = 900;
double SCALE_X = 1.0f;
double SCALE_Y = 1.0f;
// mouse
double mouse_xpos = SCR_WIDTH / 2.0f;
double mouse_ypos = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// fps
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
int FPS;
// camera
Camera camera(glm::vec3(0.5f, 10.0f, 0.5f));
// test
bool TESTENV = true;
// 回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mousebutton_callback(GLFWwindow* window, int button, int action, int mods);
void cursorpos_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
//工具函数
glm::vec4 screenToWorld(double mouse_xpos, double mouse_ypos);
// 初始化OpenGL
bool InitOpenGL(){
	if (!GLContext::initGLFW()) {
		return false;
	}
	context.create(SCR_WIDTH, SCR_HEIGHT);
	//把OpenGL的函数指针导入给GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD");
		return false;
	}
	//测试环境开启鼠标捕获
	if (TESTENV) {
		//设置鼠标捕获
		glfwSetInputMode(context.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	//设置各种回调
	glfwSetKeyCallback(context.getWindow(), key_callback);
	glfwSetMouseButtonCallback(context.getWindow(), mousebutton_callback);
	glfwSetCursorPosCallback(context.getWindow(), cursorpos_callback);
	glfwSetScrollCallback(context.getWindow(), scroll_callback);
	//开启深度测试
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//开启模板测试
	glEnable(GL_STENCIL_TEST);
	//开启混合
	glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return true;
}
// 初始化实体
void InitObject() {
	//mesh
	MeshModel* meshmodel = new MeshModel(50, 1);
	meshmodel->UpdataMesh(glm::vec3(0.0f));
	ModelManager::Register(meshmodel, "mesh");
	ShaderManager::LoadShader(MeshModel::vsPath.c_str(), MeshModel::fsPath.c_str(), nullptr, "mesh");
	VertexArrayManager::LoadVertexArray(meshmodel, "mesh");
	//spine-youlika
	SpineModel* spinemodel = new SpineModel("youlika", "res/spine/test.atlas", "res/spine/test.skel", DataMode::BINARY);
	spinemodel->SetState("Move", 0, 0, 0);
	spinemodel->UpdataMesh(glfwGetTime());
	ModelManager::Register(spinemodel, "youlika");
	ShaderManager::LoadShader(SpineModel::vsPath.c_str(), SpineModel::fsPath.c_str(), nullptr, "spine");
	VertexArrayManager::LoadVertexArray(spinemodel, "youlika");
	//spine-lisa
	spinemodel = new SpineModel("lisa", "res/spine/lisa.atlas", "res/spine/lisa.skel", DataMode::BINARY);
	spinemodel->SetState("Move", 0, 0, 0);
	spinemodel->UpdataMesh(glfwGetTime());
	ModelManager::Register(spinemodel, "lisa");
	VertexArrayManager::LoadVertexArray(spinemodel, "lisa");
	//outline
	OutLineModel* outlinemodel = new OutLineModel();
	ModelManager::Register(outlinemodel, "outline");
	ShaderManager::LoadShader(OutLineModel::vsPath.c_str(), OutLineModel::fsPath.c_str(), nullptr, "outline");
	VertexArrayManager::LoadVertexArray(outlinemodel, "outline");
	//floor
	PlaneModel* floor = new PlaneModel("res/texture/floor/marble.jpg");
	floor->SetRepeat(10, 10);
	ModelManager::Register(floor, "floor");
	ShaderManager::LoadShader(PlaneModel::vsPath.c_str(), PlaneModel::fsPath.c_str(), nullptr, "floor");
	TextureManager::LoadTexture(floor->texPath, "floor");
	VertexArrayManager::LoadVertexArray(floor, "floor");
	//button
	PlaneModel* button = new PlaneModel("res/texture/block.png");
	ModelManager::Register(button, "button");
	ShaderManager::LoadShader("res/shader/UI/ui.vs", "res/shader/UI/ui.fs", nullptr, "ui");
	TextureManager::LoadTexture(button->texPath, "button");
	VertexArrayManager::LoadVertexArray(button, "button");
	//预设置透视矩阵
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)(SCR_WIDTH * SCALE_X) / (float)(SCR_HEIGHT * SCALE_Y), 0.1f, 100.0f);
	for (auto iter : ShaderManager::Shaders) {
		iter.second.Use();
		iter.second.SetMatrix4("view", view);
		iter.second.SetMatrix4("projection", projection);
	}
}
// 更新实体
void UpdataObject() {
	//mesh
	glm::vec3 offset = static_cast<MeshModel*>(ModelManager::GetModel("mesh"))->GetOffset(camera.Position);
	glm::mat4 model(1.0f);
	model = glm::translate(model, offset);
	ShaderManager::GetShader("mesh")->Use();
	ShaderManager::GetShader("mesh")->SetMatrix4("model", model);
	VertexArrayManager::GetVertexArray("mesh")->Bind();
	ModelManager::GetModel("mesh")->GetRenderer().Render();
	//floor
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -0.01f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(20.0f, 20.0f, 1.0f));
	ShaderManager::GetShader("floor")->Use();
	ShaderManager::GetShader("floor")->SetMatrix4("model", model);
	TextureManager::GetTexture("floor")->Bind();
	VertexArrayManager::GetVertexArray("floor")->Bind();
	Renderer render = ModelManager::GetModel("floor")->GetRenderer();
	render.SetID(1);
	render.Render();
	//outline
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.5f));
	model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
	ShaderManager::GetShader("outline")->Use();
	ShaderManager::GetShader("outline")->SetMatrix4("model", model);
	VertexArrayManager::GetVertexArray("outline")->Bind();
	ModelManager::GetModel("outline")->GetRenderer().Render();
	//youlika
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.5f));
	model = glm::scale(model, glm::vec3(0.003f, 0.003f, 0.003f));
	ShaderManager::GetShader("spine")->Use();
	ShaderManager::GetShader("spine")->SetMatrix4("model", model);
	static_cast<SpineModel*>(ModelManager::GetModel("youlika"))->UpdataMesh(deltaTime);
	ModelManager::GetModel("youlika")->UpdataVertexArray(VertexArrayManager::GetVertexArray("youlika"));
	TextureManager::GetTexture("youlika")->Bind();
	VertexArrayManager::GetVertexArray("youlika")->Bind();
	render = ModelManager::GetModel("youlika")->GetRenderer();
	render.SetID(9);
	render.Render();
	//lisa
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(glfwGetTime(), 0.0f, 0.5f));
	model = glm::scale(model, glm::vec3(0.003f, 0.003f, 0.003f));
	ShaderManager::GetShader("spine")->Use();
	ShaderManager::GetShader("spine")->SetMatrix4("model", model);
	static_cast<SpineModel*>(ModelManager::GetModel("lisa"))->UpdataMesh(deltaTime);
	ModelManager::GetModel("lisa")->UpdataVertexArray(VertexArrayManager::GetVertexArray("lisa"));
	TextureManager::GetTexture("lisa")->Bind();
	VertexArrayManager::GetVertexArray("lisa")->Bind();
	render = ModelManager::GetModel("lisa")->GetRenderer();
	render.SetID(10);
	render.Render();
	//button
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.9f, 0.8f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f * (float)(SCR_WIDTH * SCALE_X) / (float)(SCR_HEIGHT * SCALE_Y), 0.1f));
	ShaderManager::GetShader("ui")->Use();
	ShaderManager::GetShader("ui")->SetMatrix4("model", model);
	TextureManager::GetTexture("button")->Bind();
	VertexArrayManager::GetVertexArray("button")->Bind();
	render = ModelManager::GetModel("button")->GetRenderer();
	render.SetID(5);
	render.Render();
}
//主程序入口
int main() {
	//初始化GLFW并创建GLFW窗口
	if (!InitOpenGL()) {
		return -1;
	}
	//预加载实体
	InitObject();
	//循环渲染
	while (context.next()) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(context.getWindow());

		context.beforeDrawFrame();

		UpdataObject();
		
		context.afterDrawFrame();
	}
	ShaderManager::Clear();
	TextureManager::Clear();
	VertexArrayManager::Clear();
	ModelManager::Clear();
	context.destroy();

	return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		std::cout << "渲染循环结束" << std::endl;
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		std::cout << "当前鼠标坐标：" << mouse_xpos << "," << mouse_ypos << std::endl;
	}
	else if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		TESTENV = !TESTENV;
		if (TESTENV) {
			glfwSetInputMode(context.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			firstMouse = true;
			std::cout << "测试模式启动" << std::endl;
		}
		else {
			glfwSetInputMode(context.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			std::cout << "测试模式关闭" << std::endl;
		}
	}
}
void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		GLfloat ref;
		glReadPixels(mouse_xpos, (SCR_HEIGHT * SCALE_Y)-mouse_ypos, 1, 1, GL_STENCIL_INDEX, GL_FLOAT, &ref);
		std::cout << "屏幕坐标：" << mouse_xpos << "," << mouse_ypos << "|模板缓冲值：" << ref << std::endl;

		if (ref == 1) {
			double xndc = (2 * mouse_xpos) / (SCR_WIDTH * SCALE_X) - 1;
			double yndc = 1 - (2 * mouse_ypos) / (SCR_HEIGHT * SCALE_Y);

			glm::vec4 world = screenToWorld(mouse_xpos, mouse_ypos);

			std::cout << "世界坐标：" << world.x << "," << world.y << "," << world.z << std::endl;
			int x = floor(world.x);
			int z = floor(world.z);

			std::cout << "网格坐标：" << x << "," << z << std::endl;
			//vec3 position(mesh_x + xoffset + 0.5, camera.Position.y, mesh_y + zoffset + 0.5);
		}
		else if (ref == 9) {

		}
	}
}
void cursorpos_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xposf = static_cast<float>(xpos);
	float yposf = static_cast<float>(ypos);
	if (firstMouse)
	{
		mouse_xpos = xposf;
		mouse_ypos = yposf;
		firstMouse = false;
	}
	float xoffset = xposf - mouse_xpos;
	float yoffset = mouse_ypos - yposf; // reversed since y-coordinates go from bottom to top
	mouse_xpos = xpos;
	mouse_ypos = ypos;

	if (TESTENV) {
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	else {
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			camera.ProcessMouseMovement(xoffset, 0, yoffset);
		}
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}
void processInput(GLFWwindow* window)
{
	if (TESTENV) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera.ProcessKeyboard(UP, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			camera.ProcessKeyboard(DOWN, deltaTime);
	}
	else {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
	}
 }

glm::vec4 screenToWorld(double mouse_xpos,double mouse_ypos) {
	//NDC
	double xndc = (2.0f * mouse_xpos) / (SCR_WIDTH * SCALE_X) - 1.0f;
	double yndc = 1.0f - (2.0f * mouse_ypos) / (SCR_HEIGHT * SCALE_Y);
	glm::vec4 proj_p = glm::vec4(xndc, yndc, 1.0f, 1.0f);
	//Projection
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)(SCR_WIDTH * SCALE_X) / (float)(SCR_HEIGHT * SCALE_Y), 0.1f, 100.0f);
	glm::mat4 inv_proj = glm::inverse(projection);
	glm::vec4 view_p = inv_proj * proj_p;
	//View
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 inv_view = glm::inverse(view);
	glm::vec4 world_p = inv_view * view_p;
	if (world_p.y != 0.0f) {
		world_p =  world_p / world_p.w;
		world_p.x = world_p.x - camera.Position.x;
		world_p.y = world_p.y - camera.Position.y;
		world_p.z = world_p.z - camera.Position.z;
		world_p = camera.Position.y * world_p / world_p.y;
		world_p.x = camera.Position.x - world_p.x;
		world_p.y = camera.Position.y - world_p.y;
		world_p.z = camera.Position.z - world_p.z;
	}
	return world_p;
}