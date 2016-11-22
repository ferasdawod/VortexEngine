#include "IEventData.h"
#include "Actors\ActorsDefenitions.h"
#include "Utilities\Utils.h"

class RenderRequest;
using RenderRequestID = unsigned long;
class Camera;
using WeakCameraPtr = std::weak_ptr<Camera>;
class LightComponent;
using WeakLightPtr = std::weak_ptr<LightComponent>;
class SettingsManager;

#define DECLARE_ID() public: static const EventID kEventID
#define DEFINE_ID(type, x) __declspec(selectany) const EventID type::kEventID = (x)

class Event_NewRenderRequest : public BaseEventData
{
	DECLARE_ID();

public:
	Event_NewRenderRequest(std::weak_ptr<RenderRequest> renderRequest)
		: BaseEventData(STRING(Event_NewRenderRequest), kEventID), _pRenderRequest(renderRequest)
	{
	}
	virtual ~Event_NewRenderRequest() {}

	std::weak_ptr<RenderRequest>	GetRenderRequest() const { return _pRenderRequest; }

protected:
	std::weak_ptr<RenderRequest>	_pRenderRequest;
};

class Event_SetActiveCamera : public BaseEventData
{
	DECLARE_ID();

public:
	Event_SetActiveCamera(WeakCameraPtr camera, ActorID actorID)
		: BaseEventData(STRING(Event_SetActiveCamera), kEventID), _pCamera(camera), _actorID(actorID)
	{	}

	virtual ~Event_SetActiveCamera() {}

	ActorID					GetActorID() const { return _actorID; }
	WeakCameraPtr			GetCamera() const { return _pCamera; }

protected:
	WeakCameraPtr			_pCamera;
	ActorID					_actorID;
};

class Event_WindowResized : public BaseEventData
{
	DECLARE_ID();

public:
	Event_WindowResized(int width, int height)
		: BaseEventData(STRING(Event_WindowResized), kEventID), _nWidth(width), _nHeight(height)
	{	}
	virtual ~Event_WindowResized() {}

	int		Width() const { return _nWidth;	}
	int		Height() const { return _nHeight; }

	virtual const std::string& ToString() const override
	{
		static std::string data;
		data = "New Dimensions : (" + std::to_string(_nWidth) + "," + std::to_string(_nHeight) + ").";
		
		return data;
	}

protected:
	int		_nWidth;
	int		_nHeight;
};

class Event_NewLight : public BaseEventData
{
	DECLARE_ID();

public:
	Event_NewLight(WeakLightPtr light, ActorID ownerID) : BaseEventData(STRING(Event_NewLight), kEventID),
		_pLight(light), _ownerID(ownerID) {}
	virtual ~Event_NewLight() {}

	WeakLightPtr	LightPtr() const { return _pLight; }
	ActorID			OwnerID() const { return _ownerID; }
protected:
	WeakLightPtr	_pLight;
	ActorID			_ownerID;
};

class Event_ApplicationExiting : public BaseEventData
{
	DECLARE_ID();

public:
	Event_ApplicationExiting() : BaseEventData(STRING(Event_ApplicationExiting), kEventID) {}
	virtual ~Event_ApplicationExiting() {	}
};

class Event_SavingSettings : public BaseEventData
{
	DECLARE_ID();

public:
	Event_SavingSettings(std::weak_ptr<SettingsManager> settingsManager) : BaseEventData(STRING(Event_SavingSettings), kEventID),
		_pSettingsManager(settingsManager) {	}
	virtual ~Event_SavingSettings() {	}

	std::weak_ptr<SettingsManager>	GetSettingsManager() const { return _pSettingsManager; }

protected:
	std::weak_ptr<SettingsManager>	_pSettingsManager;
};

class Event_ReloadMaterial : public BaseEventData
{
	DECLARE_ID();

public:
	Event_ReloadMaterial(const std::string& matName = "") : BaseEventData(STRING(Event_ReloadMaterial), kEventID), _sMaterialName(matName) {}
	virtual ~Event_ReloadMaterial() {}

	const std::string&		MaterialName() const { return _sMaterialName; }

protected:
	std::string	_sMaterialName;
};

DEFINE_ID(Event_NewRenderRequest,	0xC5D615FA);
DEFINE_ID(Event_SetActiveCamera,	0x3F558BD2);
DEFINE_ID(Event_WindowResized,		0x825C5666);
DEFINE_ID(Event_NewLight,			0xEA643318);
DEFINE_ID(Event_ApplicationExiting,	0x0C10DB00);
DEFINE_ID(Event_SavingSettings,		0xBEE3D2F1);
DEFINE_ID(Event_ReloadMaterial,		0x139FF06C);

