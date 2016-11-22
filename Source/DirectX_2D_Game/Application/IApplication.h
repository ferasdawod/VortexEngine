#pragma once

#define WIN32_LEAN_AND_MEAN			// removes unnecessary stuff
#include <Windows.h>

class IApplication
{
public:
	IApplication() {}
	virtual ~IApplication() {}

	virtual bool			Run(HINSTANCE hInstance) = 0;

protected:
	virtual bool			Init() = 0;
	virtual bool			LoadResources() = 0;
							
	virtual void			OnUpdate() = 0;
	virtual void			OnRender() = 0;
							
	virtual void			OnResize(int newWidth, int newHeight) = 0;
							
	virtual void			OnShutDown() = 0;
};