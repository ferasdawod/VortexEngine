#pragma once

#include <3rd Party/Lua/include/lua.hpp>
#include <3rd Party/LuaBridge/Source/LuaBridge/LuaBridge.h>

using namespace luabridge;

class ScriptInterface;

namespace Core
{
	class Script
	{
	public:
		Script(LuaRef self, LuaRef updateFunction, LuaRef initializeFunction, LuaRef scriptInterfaceFunction);
		virtual ~Script() {}


		void CallUpdate(float deltaTime) const;
		void CallInitialize() const;

		void SetScriptInterface(ScriptInterface* script_interface) const;

	protected:
		LuaRef _self;
		LuaRef _updateFunction;
		LuaRef _initializeFunction;

		LuaRef _scriptInterfaceFunction;
	};
}