#include "Utils.h"

#include <Windows.h>

#include <DirectXTK/SimpleMath.h>
#include <assimp/types.h>

Vector3 Utils::aiVec2Vector3(const aiVector3D& vec)
{
	return Vector3(vec.x, vec.y, vec.z);
}

Vector2 Utils::aiVec2Vector2(const aiVector3D& vec)
{
	return Vector2(vec.x, vec.y);
}

std::string Utils::WideStr2Str(const std::wstring& ws)
{
	int wsLength = (int)ws.length() + 1;
	int sLength = WideCharToMultiByte(CP_ACP, 0, ws.c_str(), wsLength, 0, 0, 0, 0) - 1;
	std::string s(sLength, '\0');
	WideCharToMultiByte(CP_ACP, 0, ws.c_str(), wsLength, &s[0], sLength, 0, 0);
	return s;
}

std::wstring Utils::Str2WideStr(const std::string& s)
{
	int slength = (int)s.length() + 1;
	int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0) - 1;
	std::wstring r(len, '\0');
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &r[0], len);
	return r;
}

bool Utils::WildcardMatch(const std::string& pattern, const std::string& path)
{
	const char* pat = pattern.c_str();
	const char* str = path.c_str();

	int i, star;

new_segment:

	star = 0;
	if (*pat == '*')
	{
		star = 1;
		do { pat++; }
		while (*pat == '*'); /* enddo */
	} /* endif */

test_match:

	for (i = 0; pat[i] && (pat[i] != '*'); i++)
	{
		//if (mapCaseTable[str[i]] != mapCaseTable[pat[i]]) {
		if (str[i] != pat[i])
		{
			if (!str[i]) return false;
			if ((pat[i] == '?') && (str[i] != '.')) continue;
			if (!star) return false;
			str++;
			goto test_match;
		}
	}
	if (pat[i] == '*')
	{
		str += i;
		pat += i;
		goto new_segment;
	}
	if (!str[i]) return true;
	if (i && pat[i - 1] == '*') return true;
	if (!star) return false;
	str++;
	goto test_match;
}