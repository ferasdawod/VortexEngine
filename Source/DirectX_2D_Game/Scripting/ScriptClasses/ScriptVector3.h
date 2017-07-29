#pragma once

namespace Core
{
	class ScriptVector3
	{
	public:
		ScriptVector3() : x(0), y(0), z(0) {}
		ScriptVector3(float x, float y, float z) : x(x), y(y), z(z) {}

		float x, y, z;
	};
}