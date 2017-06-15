#include "pch.h"

#include "ScriptComponent.h"

#include <3rd Party/Lua/include/lua.h>
#include <3rd Party/Lua/include/lauxlib.h>
#include <3rd Party/Lua/include/lualib.h>
#include <3rd Party/LuaBridge/Source/LuaBridge/LuaBridge.h>

ScriptComponent::ScriptComponent() : BaseComponent(STRING(ScriptComponent))
{
}

ScriptComponent::~ScriptComponent()
{
}
