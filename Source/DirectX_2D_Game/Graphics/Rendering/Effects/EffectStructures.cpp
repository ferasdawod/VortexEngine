#include "pch.h"

#include "EffectsStructures.h"

#include "Components/LightComponents/DirectionalLight.h"
#include "Components/LightComponents/PointLight.h"
#include "Components/LightComponents/SpotLight.h"

#include "Components/Camera.h"
#include "Graphics/Rendering/RenderSettings.h"

EffectDirectionalLight::EffectDirectionalLight()
: _diffuse(Vector3::One),
_strength(1.0f),
_direction(-Vector3::UnitY),
_shadowStrength(1.0f)
{
}

EffectDirectionalLight::EffectDirectionalLight(std::shared_ptr<DirectionalLight> lightPtr)
	: _diffuse(lightPtr->GetDiffuseColor().ToVector3()),
	_strength(lightPtr->GetStrength()),
	_direction(lightPtr->GetTransform().lock()->GetForward()),
_shadowStrength(lightPtr->GetShadowStrength())
{
}

EffectPointLight::EffectPointLight()
: _DiffuseColor(Vector3::One),
_Strength(1.0f),
_Position(Vector3::Zero),
_Falloff(2.0f),
_Range(5.0f),
_pad(Vector3::One)
{
}

EffectPointLight::EffectPointLight(std::shared_ptr<PointLight> pointLightPtr)
: _DiffuseColor(pointLightPtr->GetDiffuseColor().ToVector3()),
_Strength(pointLightPtr->GetStrength()),
_Position(pointLightPtr->GetTransform().lock()->GetPosition()),
_Falloff(pointLightPtr->GetFalloff()),
_Range(pointLightPtr->GetRange()),
_pad(Vector3::Zero)
{
}

EffectSpotLight::EffectSpotLight()
: _DiffuseColor(Vector3::One),
_Strength(1.0f),
_Position(Vector3::Zero),
_ConeAngle(30.0f),
_Direction(Vector3::UnitZ * -1.0f),
_Falloff(20.0f)
{
}

EffectSpotLight::EffectSpotLight(std::shared_ptr<SpotLight> spotLightPtr)
: _DiffuseColor(spotLightPtr->GetDiffuseColor().ToVector3()),
_Strength(spotLightPtr->GetStrength()),
_ConeAngle(DirectX::XMConvertToRadians(spotLightPtr->GetConeAngleDegrees())),
_Falloff(spotLightPtr->GetFalloff())
{
	auto transform = spotLightPtr->GetTransform().lock();
	assert(transform);
	_Position = transform->GetPosition();
	_Direction = transform->GetForward();
}

EffectPerFrameBuffer::EffectPerFrameBuffer(std::shared_ptr<Camera> camera, LightsMap& lightsMap, const Color& ambientColor,
										   std::shared_ptr<RenderSettings> renderSettings)
: _ViewProjectionMatrix(camera->GetViewMatrix() * camera->GetProjectionMatrix()),
_AmbientColor(ambientColor.ToVector4()),
_CameraPositionW(camera->GetTransform().lock()->GetPosition()),
_DirectionalCount(0),
_SpotCount(0),
_PointCount(0),
_pad0(0.0f),
_pad1(0)
{
	if (!renderSettings->LightingEnabled)
		return;

	auto cameraTransform = camera->GetTransform().lock();
	assert(cameraTransform);

	for (auto it = lightsMap.begin(); it != lightsMap.end(); ++it)
	{
		if (it->expired())
		{
			it = lightsMap.erase(it);
			if (it == lightsMap.end())
				return;
			else
				continue;
		}

		auto lightPtr = it->lock();
		assert(lightPtr != nullptr);

		if (!lightPtr->IsEnabled() || !lightPtr->GetOwner().lock()->IsEnabled())
			continue;

		auto lightType = lightPtr->GetLightType();
		if (lightType == LightType::Directional && _DirectionalCount < MAX_DIRECTIONAL)
		{
			auto directionalLight = std::static_pointer_cast<DirectionalLight>(lightPtr);
			auto lightTrans = directionalLight->GetTransform().lock();
			assert(lightTrans);

			// only the first directional light casts shadows
			if (_DirectionalCount == 0 && renderSettings->ShadowsEnabled)
			{
				// our current world is a 14x14 grid so the far corner is according to Pythagoras theory is sqrt(x*x + y*y)
				// this should be calculated based on the level
				auto sceneBounds = sqrtf(10.0f * 10.0f + 10.0f * 10.0f);

				auto shadowDistance = 2.5f;

				// VIEW MATRIX
				auto lootAtPosition = cameraTransform->GetPosition() + cameraTransform->GetForward() * shadowDistance * 0.5f;
				auto pos = lootAtPosition + -1.0f * sceneBounds * lightTrans->GetForward();
				lightTrans->SetPosition(pos);

				// the look at target should be the scene center
				//Matrix lightViewMatrix = Matrix::CreateLookAt(pos, Vector3::Zero, Vector3::Up);
				auto lightViewMatrix = Matrix::CreateLookAt(pos, lootAtPosition, Vector3::Up);


				// PROJECTION MATRIX
				//Vector3 center = Vector3::Transform(Vector3::Zero, lightViewMatrix);
				Vector3 center = Vector3::Transform(lootAtPosition, lightViewMatrix);


				float l = center.x - sceneBounds;
				float b = center.y - sceneBounds;
				float n = center.z - sceneBounds;
				float r = center.x + sceneBounds;
				float t = center.y + sceneBounds;
				float f = center.z + sceneBounds;

				Matrix lightProjectionMatrix = Matrix::CreateOrthographicOffCenter(l, r, b, t, -f, -n);

				_LightViewProjectionMatrix = lightViewMatrix * lightProjectionMatrix;
			}

			_DirectionalLights[_DirectionalCount] = EffectDirectionalLight(directionalLight);
			_DirectionalCount++;
		}
		else if (lightType == LightType::Point && _PointCount < MAX_POINT)
		{
			auto pointLightPtr = std::static_pointer_cast<PointLight>(lightPtr);
			
			_PointLights[_PointCount] = EffectPointLight(pointLightPtr);
			_PointCount++;
		}
		else if (lightType == LightType::Spot && _SpotCount < MAX_SPOT)
		{
			auto spotLightPtr = std::static_pointer_cast<SpotLight>(lightPtr);

			_SpotLights[_SpotCount] = EffectSpotLight(spotLightPtr);
			_SpotCount++;
		}
	}
}

EffectSettingsBuffer::EffectSettingsBuffer(std::shared_ptr<RenderSettings> renderSettings) :
_LightingEnabled(renderSettings->LightingEnabled),
_SoftShadowsEnabled(renderSettings->SoftShadowsEnabled),
_TexturingEnabled(renderSettings->TexturingEnabled),
_ShadowsEnabled(renderSettings->ShadowsEnabled)
{
}
