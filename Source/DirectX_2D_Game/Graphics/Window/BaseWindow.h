#pragma once

#include "IWindow.h"

class BaseWindow : public IWindow
{
public:
	BaseWindow(const std::string& title);
	~BaseWindow();


};