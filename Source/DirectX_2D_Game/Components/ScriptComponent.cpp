#include "pch.h"

#include "ScriptComponent.h"

#include <3rd Party/Lua/include/lua.hpp>
#include <3rd Party/LuaBridge/Source/LuaBridge/LuaBridge.h>
#include "Scripting/ScriptManager.h"
#include <Scripting/ScriptInterface.h>

ScriptComponent::ScriptComponent() : BaseComponent(STRING(ScriptComponent))
{
}

ScriptComponent::~ScriptComponent()
{
}

void ScriptComponent::Initialize()
{
	BaseComponent::Initialize();

	_script = Core::ScriptManager::GetPtr()->LoadScript(_scriptPath);
	if (!_script)
	{
		LOG_W("Failed to load script");
		return;
	}

	_scriptInterface.reset(DBG_NEW ScriptInterface(GetOwner().lock()));
	_script->SetScriptInterface(_scriptInterface.get());

	_script->CallInitialize();
}

void ScriptComponent::Initialize(TiXmlElement * xmlData)
{
	BaseComponent::Initialize(xmlData);

	XmlHelper::FromXml(xmlData, "ScriptFilePath", _scriptPath);
}

void ScriptComponent::OnUpdate(float deltaTime)
{
	FUNC_PROFILE();

	if (_script)
		_script->CallUpdate(deltaTime);
}

TiXmlElement* ScriptComponent::ToXML() const
{
	return BaseComponent::ToXML();
	// TODO complete this
}

void ScriptComponent::RegisterProperties()
{
	RegisterProperty("Script Path", Core::PropertyType::String, &_scriptPath);
}

void ScriptComponent::ReloadScript()
{
	_script.release();
	_script = Core::ScriptManager::GetPtr()->LoadScript(_scriptPath);
	if (!_script)
	{
		LOG_W("Failed to load script");
		return;
	}
	_script->SetScriptInterface(_scriptInterface.get());
	_script->CallInitialize();
}
