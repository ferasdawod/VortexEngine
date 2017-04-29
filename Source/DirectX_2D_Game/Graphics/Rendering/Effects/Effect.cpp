#include "pch.h"

#include "Effect.h"

#include "Utilities/D3DUtils.h"

#include "Graphics/Rendering/GraphicsDevice.h"
#include <FX11/d3dx11effect.h>
#include "Graphics/Material.h"
#include "Components/Camera.h"
#include "Graphics/Rendering/RenderRequest.h"
#include "Graphics/Rendering/ShadowMap.h"
#include "Graphics/Rendering/RenderSettings.h"
#include "Graphics/Vertex.h"

#include "Engine/SettingsManager.h"

#include "Components/LightComponents/DirectionalLight.h"

#include "Resources/ResCache.h"
#include "Resources/ResHandle.h"

#include "Graphics/Texture2D.h"

#include <DirectXTK/DirectXHelpers.h>
#include "EffectsStructures.h"

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;

Effect::Effect(std::shared_ptr<RenderSettings> renderSettings) :
_pGraphicsDevice(nullptr), _pEffect(nullptr), _pInputLayout(nullptr),
_pMainTechnique(nullptr), _pShadowPassTechnique(nullptr), _pMaterialBuffer(nullptr), _pMatricesBuffer(nullptr),
_pPerFrameBuffer(nullptr), _currentTech(RenderTechnique::MainTech), _pDiffuseTexture(nullptr), _pDiffuseSampler(nullptr),
_pSpecularTexture(nullptr), _pNormalMapTexture(nullptr), _pShadowMapTexture(nullptr), _pShadowSampler(nullptr), _pSettingsBuffer(nullptr)
{
	assert(renderSettings);
	_pRenderSettings = renderSettings;
}

Effect::~Effect()
{
	ReleaseResources();
}

void Effect::ReleaseResources()
{
	_pMainTechnique = nullptr;
}

#pragma region Effect File Initialization

bool Effect::Init(const std::string& fileName, GraphicsDevice* graphicsDevice)
{
	ReleaseResources();

	_pGraphicsDevice = graphicsDevice;

	auto br = LoadEffect(fileName, graphicsDevice);
	if (!br)
	{
		LOG_E("Failed to load the effect file", 0);
		return false;
	}

	_pMainTechnique = _pEffect->GetTechniqueByName("MainTech");
	_pShadowPassTechnique = _pEffect->GetTechniqueByName("ShadowPassTech");

	if (!_pShadowPassTechnique || !_pMainTechnique)
	{
		LOG_E("Failed to get the effect techs", 0);
	}

	ID3DX11EffectPass* effectPass = nullptr;
	D3DX11_PASS_SHADER_DESC passDesc = { 0 };
	D3DX11_EFFECT_SHADER_DESC shaderDesc = { 0 };

	effectPass = _pMainTechnique->GetPassByIndex(0);
	effectPass->GetVertexShaderDesc(&passDesc);
	passDesc.pShaderVariable->GetShaderDesc(passDesc.ShaderIndex, &shaderDesc);

	graphicsDevice->GetDevice()->CreateInputLayout(
		Vertex::InputElements, Vertex::InputElementsCount,
		shaderDesc.pBytecode, shaderDesc.BytecodeLength, &_pInputLayout);

	DirectX::SetDebugObjectName(_pInputLayout.Get(), "TheEffectInputLayout");

	_pMaterialBuffer = _pEffect->GetConstantBufferByName("MaterialBuffer");
	_pMatricesBuffer = _pEffect->GetConstantBufferByName("MatricesBuffer");	
	_pPerFrameBuffer = _pEffect->GetConstantBufferByName("PerFrameBuffer");
	_pSettingsBuffer = _pEffect->GetConstantBufferByName("SettingsBuffer");

	_pDiffuseTexture = _pEffect->GetVariableByName("diffuseTexture")->AsShaderResource();
	_pShadowMapTexture = _pEffect->GetVariableByName("shadowMapTexture")->AsShaderResource();
	_pShadowSampler = _pEffect->GetVariableByName("shadowSampler")->AsSampler();
	_pDiffuseSampler = _pEffect->GetVariableByName("diffuseSampler")->AsSampler();
	_pSpecularTexture = _pEffect->GetVariableByName("specularTexture")->AsShaderResource();
	_pNormalMapTexture = _pEffect->GetVariableByName("normalmapTexture")->AsShaderResource();

	if (!_pShadowMapTexture || !_pDiffuseTexture || !_pPerFrameBuffer || !_pMaterialBuffer || !_pMatricesBuffer || !_pInputLayout ||
		!_pDiffuseSampler || !_pShadowSampler || !_pSpecularTexture || !_pNormalMapTexture || !_pSettingsBuffer)
	{
		LOG_E("Failed to get the effect variables", 0);
		return false;
	}

	return true;
}

bool Effect::LoadEffect(const std::string& effectFileName, GraphicsDevice* graphicsDevice)
{
	auto br = D3DUtils::CreateEffectFromFile(Utils::Str2WideStr(effectFileName).c_str(), &_pEffect);
	if (!br)
	{
		LOG_E("Failed to create the effect", 0);
		return false;
	}

	return true;
}

#pragma endregion

void Effect::PrepareFrame(LightsMap& lightsMap, std::shared_ptr<Camera> camera, const Color& ambientColor)
{
	FUNC_PROFILE();

	EffectPerFrameBuffer perFrameBuff(camera, lightsMap, ambientColor, _pRenderSettings);
	_pPerFrameBuffer->SetRawValue(&perFrameBuff, 0, sizeof(perFrameBuff));

	EffectSettingsBuffer settingsBuff(_pRenderSettings);
	_pSettingsBuffer->SetRawValue(&settingsBuff, 0, sizeof(settingsBuff));
}

void Effect::UpdateMaterial(std::shared_ptr<Material> mat)
{
	EffectMaterialBuffer materialBuff(mat);
	_pMaterialBuffer->SetRawValue(&materialBuff, 0, sizeof(materialBuff));

	bool lightingEnabled = _pRenderSettings->LightingEnabled;
	bool texturingEnabled = _pRenderSettings->TexturingEnabled;

	if (texturingEnabled)
	{
		// setting the textures only if we are not in the shadow pass
		if (mat->GetIsTextured() && _currentTech != RenderTechnique::ShadowTech)
		{
			auto textureHandle = ResCache::GetPtr()->GetHandle(mat->GetTexturePath());
			auto texture = std::static_pointer_cast<Texture2D>(textureHandle->GetResource().lock());

			if (!texture)
				LOG_E("This material is textured but no texture is loaded for it bro", 0);

			_pDiffuseTexture->SetResource(texture->GetShaderResourceView().Get());
			_pDiffuseSampler->SetSampler(0, _pGraphicsDevice->GetSampler(mat->GetTextureSampler()).Get());
		}
		if (mat->GetUseSpecularTexture() && lightingEnabled && _currentTech != RenderTechnique::ShadowTech)
		{
			auto textureHandle = ResCache::GetPtr()->GetHandle(mat->GetSpecularTexturePath());
			auto texture = std::static_pointer_cast<Texture2D>(textureHandle->GetResource().lock());

			_pSpecularTexture->SetResource(texture->GetShaderResourceView().Get());
		}
		if (mat->GetUseNormalMap() && lightingEnabled && _currentTech != RenderTechnique::ShadowTech)
		{
			auto textureHandle = ResCache::GetPtr()->GetHandle(mat->GetNormalMapTexturePath());
			auto texture = std::static_pointer_cast<Texture2D>(textureHandle->GetResource().lock());

			_pNormalMapTexture->SetResource(texture->GetShaderResourceView().Get());
		}
	}
}

void Effect::UpdateMatrices(std::shared_ptr<Transform> transform)
{
	EffectMatricesBuffer matricesBuff(transform->GetWorldMat());
	_pMatricesBuffer->SetRawValue(&matricesBuff, 0, sizeof(matricesBuff));
}

bool Effect::Apply()
{
	_pGraphicsDevice->GetContext()->IASetInputLayout(_pInputLayout.Get());
	
	HRESULT hr = S_OK;
	if (_currentTech == RenderTechnique::MainTech)
		hr = _pMainTechnique->GetPassByIndex(0)->Apply(0, _pGraphicsDevice->GetContext().Get());
	else
		hr = _pShadowPassTechnique->GetPassByIndex(0)->Apply(0, _pGraphicsDevice->GetContext().Get());
	
	if (FAILED(hr)) return false;

	return true;
}

void Effect::SetTechnique(RenderTechnique tech)
{
	_currentTech = tech;
}

void Effect::SetShadowMap(std::shared_ptr<ShadowMap> shadowMap)
{
	_pShadowMapTexture->SetResource(shadowMap->GetShaderResourceView().Get());
	_pShadowSampler->SetSampler(0, _pGraphicsDevice->GetSampler(SamplingType::PointClamp).Get());
	_pMainTechnique->GetPassByIndex(0)->Apply(0, _pGraphicsDevice->GetContext().Get());
}

void Effect::RemoveShadowMap()
{
	_pShadowMapTexture->SetResource(nullptr);
	_pMainTechnique->GetPassByIndex(0)->Apply(0, _pGraphicsDevice->GetContext().Get());
}
