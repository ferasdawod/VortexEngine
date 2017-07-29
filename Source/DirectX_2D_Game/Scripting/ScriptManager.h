#pragma once

#include <Utilities/Singleton.h>
#include "Script.h"
#include <memory>

struct lua_State;

namespace Core
{
	class ScriptManager : public Singleton<ScriptManager>
	{
	public:
		ScriptManager();
		virtual ~ScriptManager();

		bool Initialize();
		std::unique_ptr<Script> LoadScript(const std::string& filePath) const;

		lua_State* GetLuaState() const { return L; }

	protected:
		void RegisterCppClasses() const;

	protected:
		lua_State* L;
	};
}