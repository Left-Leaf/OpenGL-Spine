#pragma once
#include <map>
#include <string>

#include <GUI/UITree.h>

class UIManager
{
private:
	static UITree uiTree;
public:
	static void RegistUI(UI* ui);
	static UI* GetClickUI(const float& x, const float& y);
};

