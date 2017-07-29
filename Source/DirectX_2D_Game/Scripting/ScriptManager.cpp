#include "pch.h"

#include "ScriptManager.h"

#include <filesystem>
#include <Scripting/ScriptInterface.h>

#include <Scripting/ScriptClasses/ScriptVector3.h>

Core::ScriptManager::ScriptManager()
{
}


Core::ScriptManager::~ScriptManager()
{
	lua_close(L);
}

bool Core::ScriptManager::Initialize()
{
	L = luaL_newstate();
	if (L == nullptr)
	{
		LOG_E("Failed to create new lua state");
		return false;
	}
	luaL_openlibs(L);

	RegisterCppClasses();
}

std::unique_ptr<Core::Script> Core::ScriptManager::LoadScript(const std::string & filePath) const
{
	// first load the file
	if (luaL_dofile(L, filePath.c_str()) != 0)
	{
		LOG_W("Failed to load lua file" + filePath);
		LOG_W(lua_tostring(L, -1));

		return nullptr;
	}

	// get the class name which should match the file name
	std::experimental::filesystem::path path(filePath);
	auto className = path.stem().string();

	// get a reference to the lua class
	auto luaClass = getGlobal(L, className.c_str());
	if (luaClass.isNil())
	{
		LOG_W("Failed to get the class from file: " + filePath);
		return nullptr;
	}

	// get a reference to the create function of the class
	auto createFunction = luaClass["Create"];
	if (createFunction.isNil())
	{
		LOG_W("Failed to get the create function from the file: " + filePath);
		return nullptr;
	}

	// create an object from the class
	auto luaObject = createFunction(luaClass);
	if (luaObject.isNil())
	{
		LOG_W("Failed to create an object from file: " + filePath);
		return nullptr;
	}

	// get the initialize function
	auto initializeFunc = luaObject["Initialize"];
	if (initializeFunc.isNil())
	{
		LOG_W("Failed to get the initialize function from file: " + filePath);
		return nullptr;
	}

	// get the update function
	auto updateFunction = luaObject["Update"];
	if (updateFunction.isNil())
	{
		LOG_W("Failed to get the update function from file: " + filePath);
		return nullptr;
	}

	auto scriptInterfaceFunction = luaObject["SetGameInterface"];
	if (scriptInterfaceFunction.isNil())
	{
		LOG_W("Failed to get the script interface function from file: " + filePath);
		return nullptr;
	}

	return std::unique_ptr<Script>(DBG_NEW Script(luaObject, updateFunction, initializeFunc, scriptInterfaceFunction));
}

void Core::ScriptManager::RegisterCppClasses() const
{
	getGlobalNamespace(L)
		.beginClass<ScriptInterface>("ScriptInterface")
			.addFunction("GetXPosition", &ScriptInterface::GetXPosition)
			.addFunction("GetYPosition", &ScriptInterface::GetYPosition)
			.addFunction("GetZPosition", &ScriptInterface::GetZPosition)
			.addFunction("GetPosition", &ScriptInterface::GetPosition)
			.addFunction("Move", &ScriptInterface::Move)
			.addFunction("Rotate", &ScriptInterface::Rotate)
		.endClass()

		.beginClass<ScriptVector3>("ScriptVector3")
			.addConstructor<void (*) ()>()
			.addConstructor<void (*) (float, float, float)>()
			.addData("x", &ScriptVector3::x)
			.addData("y", &ScriptVector3::y)
			.addData("z", &ScriptVector3::z)
		.endClass();
}
