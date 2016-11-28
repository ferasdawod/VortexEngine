#pragma once

#include <Utilities/Macros.h>

class ViewPort
{
public:

	// initialize a new view port object
	// note all parameters are interpreted as percentages of the screen
	// for example: ViewPort(0.5f, 0.5f, 0.5f, 0.5f) creates a view port covering the lower right portion of the screen
	ViewPort(float x, float y, float width, float height)
		: _X(x),
		  _Y(y),
		  _Width(width),
		  _Height(height)
	{
	}

	ViewPort() : ViewPort(0.f, 0.f, 1.f, 1.f) {}

	void Reset(float x, float y, float width, float height)
	{
		_X = x;
		_Y = y;
		_Width = width;
		_Height = height;
	}

	DECLARE_PROPERTY(float, X);
	DECLARE_PROPERTY(float, Y);
	DECLARE_PROPERTY(float, Width);
	DECLARE_PROPERTY(float, Height);
};
