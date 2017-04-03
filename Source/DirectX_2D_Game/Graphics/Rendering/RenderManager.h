#pragma once

#include <memory>
#include <list>
#include <unordered_map>
#include <vector>
#include <functional>

#include "Actors/ActorsDefenitions.h"
#include "Events/EventListener.h"

#include <d3d11.h>
#include "RenderSettings.h"

class RenderRequest;
using WeakRenderRequestPtr = std::weak_ptr<RenderRequest>;
using StrongRenderRequestPtr = std::shared_ptr<RenderRequest>;

using RenderRequests = std::list<WeakRenderRequestPtr>;

class Camera;
// update this in Event types file when changing this
using WeakCameraPtr = std::weak_ptr<Camera>;

class Effect;
class GraphicsDevice;
class ShadowMap;
class GameTimer;
class SubMesh;
class Material;
class Transform;

class LightComponent;
using WeakLightPtr = std::weak_ptr<LightComponent>;
using LightsMap = std::list<WeakLightPtr>;


class RenderManager : public EventListener
{
public:
	explicit		RenderManager();
	virtual			~RenderManager();

	bool			Initialize(HWND hWnd);
	void			OnRender();

	void			ToggleWireframe() { _pRenderSettings->WireframeEnabled = !_pRenderSettings->WireframeEnabled; }
	void			ToggleLighting() { _pRenderSettings->LightingEnabled = !_pRenderSettings->LightingEnabled; }
	void			ToggleTexturing() { _pRenderSettings->TexturingEnabled = !_pRenderSettings->TexturingEnabled; }
	void			ToggleShadows() { _pRenderSettings->ShadowsEnabled = !_pRenderSettings->ShadowsEnabled; }
	void			ToggleSoftShadows() { _pRenderSettings->SoftShadowsEnabled = !_pRenderSettings->SoftShadowsEnabled; }

private:
	void			OnResize(int width, int height);
	
	bool			ValidateRenderRequest(StrongRenderRequestPtr renderRequest, std::shared_ptr<Camera> camera) const;
	bool			IsVisible(std::shared_ptr<SubMesh> subMesh, std::shared_ptr<Transform> transform, std::shared_ptr<Camera> camera);
	void			ProcessDrawItem(std::shared_ptr<Material> mat, std::shared_ptr<SubMesh> subMesh, std::shared_ptr<Transform> transform);

	void			RenderToShadowMap(std::shared_ptr<Camera> camera);
	void			RenderToBackBuffer(std::shared_ptr<Camera> camera);
	void			Render(std::function< bool(std::shared_ptr<Material>, std::shared_ptr<SubMesh>) > validationCallback, std::shared_ptr<Camera> camera);

	virtual bool	HandleEvent(StrongEventDataPtr eventData) override;
	virtual void	HandleRegistering(bool isRegistering) override;

private:
	RenderRequests						_renderRequests;

	LightsMap							_lightsMap;
	std::shared_ptr<ShadowMap>			_pShadowMap;

	std::unique_ptr<GraphicsDevice>		_pGraphicsDevice;

	std::vector<WeakCameraPtr>			_cameras;

	std::shared_ptr<RenderSettings>	_pRenderSettings;

	std::unique_ptr<Effect>				_pEffect;
};