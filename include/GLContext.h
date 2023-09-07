#ifndef _GLCONTEXT_H
#define _GLCONTEXT_H

#include <GLFW/glfw3.h>

extern int SCR_WIDTH;
extern int SCR_HEIGHT;
extern double SCALE_X;
extern double SCALE_Y;

class GLContext
{
public:
    static bool initGLFW();

    bool create(int width, int height);
    void destroy();

    GLFWwindow* getWindow();

    void beforeDrawFrame();
    void afterDrawFrame();
    bool next();

private:
    GLFWwindow* _window;
    int _winWidth, _winHeight;
    int _fbWidth, _fbHeight;
    bool _depth, _stencil, _blend;
};

#endif

