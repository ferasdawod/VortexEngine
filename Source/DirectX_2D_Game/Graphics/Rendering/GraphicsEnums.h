#pragma once

enum class SamplingType
{
	PointClamp,
	PointWrap,
	LinearClamp,
	LinearWrap,
	AnisotropicClamp,
	AnisotropicWrap,
};

enum class DepthState
{
	Default,
	Read,
	None,
};

enum class CullState
{
	None,
	ClockWise,
	CounterClockWise,
	Wireframe,
};

enum class AlphaState
{
	AlphaBlend,
	Additive,
	NonPremultiplied,
	Opaque,
};