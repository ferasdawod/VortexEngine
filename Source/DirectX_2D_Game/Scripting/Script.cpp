#include "pch.h"

#include "Script.h"

#include <3rd Party/Lua/include/lua.hpp>
#include <3rd Party/LuaBridge/Source/LuaBridge/LuaBridge.h>
#include "ScriptManager.h"

using namespace luabridge;

Core::Script::Script(LuaRef self, LuaRef updateFunction, LuaRef initializeFunction, LuaRef scriptInterfaceFunction)
	: _self(self), _updateFunction(updateFunction), _initializeFunction(initializeFunction), _scriptInterfaceFunction(scriptInterfaceFunction)
{
}

void Core::Script::CallUpdate(float deltaTime) const
{
	if (_updateFunction.isFunction()) _updateFunction(_self, deltaTime);
}

void Core::Script::CallInitialize() const
{
	if (_initializeFunction.isFunction()) _initializeFunction(_self);
}

void Core::Script::SetScriptInterface(ScriptInterface* script_interface) const
{
	LuaRef v(ScriptManager::GetPtr()->GetLuaState(), script_interface);

	if (_scriptInterfaceFunction.isFunction()) 
		_scriptInterfaceFunction(_self, v);
}
