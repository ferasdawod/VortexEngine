#pragma once

#include "BaseComponent.h"

namespace Core
{
	class Script;
}
class ScriptInterface;

class ScriptComponent : public BaseComponent
{
public:
	static const ComponentTypeId	kComponentID;
	EXPOSE_ID(kComponentID);

public:
	ScriptComponent();
	virtual ~ScriptComponent();

	virtual void	Initialize() override;
	virtual void	Initialize(TiXmlElement* xmlData) override;

	virtual void	OnUpdate(float deltaTime) override;

	virtual TiXmlElement* ToXML() const override;
	virtual void RegisterProperties() override;

	const string& GetScriptFilePath() const { return _scriptPath; }
	void SetScriptFilePath(const string& scriptPath) { _scriptPath = scriptPath; }

	void ReloadScript();

protected:
	string _scriptPath;
	std::unique_ptr<Core::Script> _script;
	std::shared_ptr<ScriptInterface> _scriptInterface;
};

__declspec(selectany) const ComponentTypeId ScriptComponent::kComponentID = 0x93364454;