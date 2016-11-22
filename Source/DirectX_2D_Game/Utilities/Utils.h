#pragma once

#include <string>

template <typename T>
class aiVector3t;

typedef aiVector3t<float> aiVector3D;

#define STRING(x) std::string((#x))

namespace DirectX
{
	namespace SimpleMath
	{
		struct Vector3;
		struct Vector2;
	}
}
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;

namespace Utils
{
	Vector3				aiVec2Vector3(const aiVector3D& vec);
	Vector2				aiVec2Vector2(const aiVector3D& vec);

	std::string			WideStr2Str(const std::wstring& ws);
	std::wstring		Str2WideStr(const std::string& s);

	// The following function was found on http://xoomer.virgilio.it/acantato/dev/wildcard/wildmatch.html, where it was attributed to 
	// the C/C++ Users Journal, written by Mike Cornelison. It is a little ugly, but it is FAST. Use this as an excercise in not reinventing the
	// wheel, even if you see gotos.
	bool				WildcardMatch(const std::string& pattern, const std::string& path);
}