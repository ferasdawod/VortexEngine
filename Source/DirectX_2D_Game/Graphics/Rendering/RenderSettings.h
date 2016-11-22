#pragma once

struct RenderSettings
{
	bool	LightingEnabled;
	bool	WireframeEnabled;
	bool	TexturingEnabled;
	bool	SoftShadowsEnabled;
	bool	ShadowsEnabled;

	RenderSettings() :
		LightingEnabled(true),
		WireframeEnabled(false),
		TexturingEnabled(true),
		ShadowsEnabled(true),
		SoftShadowsEnabled(true)
	{
	}
};