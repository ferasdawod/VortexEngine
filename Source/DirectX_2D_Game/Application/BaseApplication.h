#pragma once

#include "IApplication.h"
#include <string>

class BaseApplication :	public IApplication
{
public:
	BaseApplication() : _sAppName("NoTitle") {	}
	virtual ~BaseApplication() {	}

	virtual bool Run(HINSTANCE hInstace) override = 0;

protected:
	virtual bool Init() = 0;
	virtual bool LoadResources() override = 0;

	virtual void OnUpdate() override = 0;
	virtual void OnRender() override = 0;

	virtual void OnResize(int newWidth, int newHeight) override = 0;

	virtual void OnShutDown() override = 0;

protected:
	std::string							_sAppName;
};

