#pragma once

#include <string>
#include <memory>
#include <list>
#include <wrl/client.h>
#include <unordered_map>

struct ID3DX11Effect;
struct ID3DX11EffectConstantBuffer;
struct ID3DX11EffectTechnique;
struct ID3D11InputLayout;
struct ID3DX11EffectShaderResourceVariable;
struct ID3DX11EffectSamplerVariable;
struct RenderSettings;
class GraphicsDevice;
class Material;
class Transform;
class RenderRequest;
class Camera;
class ShadowMap;

class LightComponent;
using ActorID = unsigned long;
using WeakLightPtr = std::weak_ptr<LightComponent>;
using LightsMap = std::list<WeakLightPtr>;

namespace DirectX
{
	namespace SimpleMath
	{
		struct Color;
	}
}
using DirectX::SimpleMath::Color;

using Microsoft::WRL::ComPtr;

enum class RenderTechnique
{
	MainTech,
	ShadowTech,
};

class Effect
{
public:
	Effect(std::shared_ptr<RenderSettings> renderSettings);
	virtual ~Effect();

	bool			Init(const std::string& effectFileName, GraphicsDevice* graphicsDevice);

	void			PrepareFrame(LightsMap& lightsMap, std::shared_ptr<Camera> camera, const Color& ambientColor);
	bool			Apply();

	void			UpdateMaterial(std::shared_ptr<Material> mat);
	void			UpdateMatrices(std::shared_ptr<Transform> transform);
	void			SetTechnique(RenderTechnique tech);

	void			SetShadowMap(std::shared_ptr<ShadowMap> shadowMap);
	void			RemoveShadowMap();

private:
	bool			LoadEffect(const std::string& effectFileName, GraphicsDevice* graphicsDevice);

	void			ReleaseResources();

private:
	GraphicsDevice*				_pGraphicsDevice;

	std::shared_ptr<RenderSettings>	_pRenderSettings;

	ComPtr<ID3DX11Effect>		_pEffect;
	ComPtr<ID3D11InputLayout>	_pInputLayout;
	ID3DX11EffectTechnique*		_pMainTechnique;
	ID3DX11EffectTechnique*		_pShadowPassTechnique;

	ID3DX11EffectConstantBuffer*	_pMaterialBuffer;
	ID3DX11EffectConstantBuffer*	_pMatricesBuffer;
	ID3DX11EffectConstantBuffer*	_pPerFrameBuffer;
	ID3DX11EffectConstantBuffer*	_pSettingsBuffer;

	RenderTechnique					_currentTech;

	ID3DX11EffectShaderResourceVariable*		_pDiffuseTexture;
	ID3DX11EffectSamplerVariable*				_pDiffuseSampler;

	ID3DX11EffectShaderResourceVariable*		_pSpecularTexture;
	ID3DX11EffectShaderResourceVariable*		_pNormalMapTexture;

	ID3DX11EffectShaderResourceVariable*		_pShadowMapTexture;
	ID3DX11EffectSamplerVariable*				_pShadowSampler;
};