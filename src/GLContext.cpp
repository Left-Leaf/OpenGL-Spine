#include <GLContext.h>
#include <iostream>


void errorCb(int error, const char* desc) {
    std::cout << "GLFW error " << error << ": " << desc << std::endl;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    SCALE_X = width / SCR_WIDTH;
    SCALE_Y = height / SCR_HEIGHT;
    glViewport(0, 0, width, height);
}

bool GLContext::initGLFW()
{
    if (!glfwInit()) {
        std::cout << "Failed to init GLFW.";
        return false;
    }
    glfwSetErrorCallback(errorCb);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//核心模式
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    return true;
}

bool GLContext::create(int width, int height) {
    _window = glfwCreateWindow(width, height, "Test", NULL, NULL);
    if (_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(0);
    glfwSetTime(0);

    glfwGetWindowSize(_window, &_winWidth, &_winHeight);
    glfwGetFramebufferSize(_window, &_fbWidth, &_fbHeight);
    glViewport(0, 0, _fbWidth, _fbHeight);

    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

    return true;
}
void GLContext::destroy()
{
    glfwTerminate();
}
GLFWwindow* GLContext::getWindow()
{
    return _window;
}
void GLContext::beforeDrawFrame() {
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glViewport(0, 0, _fbWidth, _fbHeight);
}
void GLContext::afterDrawFrame() {
    glfwSwapBuffers(_window);
    glfwPollEvents();
}
bool GLContext::next()
{
    return !glfwWindowShouldClose(_window);
}







