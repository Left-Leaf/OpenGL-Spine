#pragma once
#include <glad/glad.h>

class Mouse {
private:
	double mouse_xpos;
	double mouse_ypos;
	bool firstMouse;
public:
	void MouseMove(double xpos, double ypos);
	void MouseClick();
};