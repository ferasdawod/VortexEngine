#pragma once

#include <DirectXTK/SimpleMath.h>
#include <unordered_map>

#include "Graphics/Material.h"

// this links to the shader in use
#define MAX_DIRECTIONAL 3
#define MAX_POINT 3
#define MAX_SPOT 3

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Color;

class LightComponent;
class DirectionalLight;
class PointLight;
class SpotLight;
class Camera;
struct RenderSettings;
using WeakLightPtr = std::weak_ptr<LightComponent>;
using LightsMap = std::list<WeakLightPtr>;
using BOOL = int;

struct EffectMaterial
{
	Vector4		diffuse;
	Vector3		specular;
	float		specularPower;
	Vector4		ambient;
	
	BOOL		isLit;
	BOOL		isTextured;
	BOOL		receiveShadows;
	BOOL		useSpecularMap;

	BOOL		useNormalMap;
	float		_pad[3];

	Matrix		textureMatrix;

	EffectMaterial(std::shared_ptr<Material> mat) :
		diffuse(mat->GetDiffuseColor().ToVector4()),
		specular(mat->GetSpecularColor().ToVector3()),
		ambient(mat->GetAmbientColor().ToVector4()),
		isLit(mat->GetIsLit()),
		isTextured(mat->GetIsTextured()),
		receiveShadows(mat->GetReceiveShadows()),
		specularPower(mat->GetSpecularPower()),
		useSpecularMap(mat->GetUseSpecularTexture()),
		useNormalMap(mat->GetUseNormalMap()),
		textureMatrix(mat->GetTextureMatrix())
	{
	}
}; 

struct EffectDirectionalLight
{
	Vector3 _diffuse;
	float	_strength;

	Vector3 _direction;
	float	_shadowStrength;

	// a default ctor is required so we can make an array of EffectDirectionalLight
	EffectDirectionalLight();
	EffectDirectionalLight(std::shared_ptr<DirectionalLight> lightPtr);
};

struct EffectPointLight
{
	Vector3	_DiffuseColor;
	float	_Strength;
	
	Vector3	_Position;
	float	_Falloff;

	float	_Range;
	Vector3	_pad;

	EffectPointLight();
	EffectPointLight(std::shared_ptr<PointLight> pointLightPtr);
};

struct EffectSpotLight
{
	Vector3	_DiffuseColor;
	float	_Strength;

	Vector3	_Position;
	float	_ConeAngle;

	Vector3	_Direction;
	float	_Falloff;

	EffectSpotLight();
	EffectSpotLight(std::shared_ptr<SpotLight> spotLightPtr);
};

struct EffectMaterialBuffer
{
	EffectMaterial	_ObjectMaterial;

	EffectMaterialBuffer(std::shared_ptr<Material> mat) : _ObjectMaterial(mat) {}
};

struct EffectMatricesBuffer
{
	Matrix			_WorldMatrix;
	Matrix			_WorldInvTransMatrix;
	
	EffectMatricesBuffer(const Matrix& worldMatrix) : _WorldMatrix(worldMatrix)
	{
		_WorldInvTransMatrix = worldMatrix;
		_WorldInvTransMatrix = _WorldInvTransMatrix.Invert();
		_WorldInvTransMatrix = _WorldInvTransMatrix.Transpose();
	}
};

struct EffectPerFrameBuffer
{
	Matrix					_ViewProjectionMatrix;
	Matrix					_LightViewProjectionMatrix;
	
	EffectDirectionalLight	_DirectionalLights[MAX_DIRECTIONAL];
	EffectPointLight		_PointLights[MAX_POINT];
	EffectSpotLight			_SpotLights[MAX_SPOT];
	
	Vector4					_AmbientColor;
	
	Vector3					_CameraPositionW;
	float					_pad0;

	int						_DirectionalCount;
	int						_PointCount;
	int						_SpotCount;
	int						_pad1;

	EffectPerFrameBuffer(std::shared_ptr<Camera> camera, LightsMap& lightsMap, const Color& ambientColor, std::shared_ptr<RenderSettings> renderSettings);
};

struct EffectSettingsBuffer
{
	BOOL	_TexturingEnabled;
	BOOL	_LightingEnabled;
	BOOL	_ShadowsEnabled;
	BOOL	_SoftShadowsEnabled;

	EffectSettingsBuffer(std::shared_ptr<RenderSettings> renderSettings);
};