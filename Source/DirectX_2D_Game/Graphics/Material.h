#pragma once

#include <DirectXTK/SimpleMath.h>
#include <string>
#include <memory>

#include "Graphics/Rendering/GraphicsEnums.h"

#include "Utilities/Macros.h"
#include "Debugging/Debug.h"

#include "Events/EventListener.h"
#include <Utilities/UniqueObject.h>

using DirectX::SimpleMath::Color;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Matrix;

class TiXmlElement;

namespace Core
{
	class GuiController;
}

class Material : public EventListener, public UniqueObject
{
public:
	friend class Core::GuiController;

	Material();
	Material(const std::string& matName);

	Material(const std::string& name,
			 const Color& diffuseColor,
			 const Color& specColor, float specPower, 
			 const Color& ambientColor,
			 bool isLit, bool isTextured, const std::string& textureName, SamplingType sampleType,
			 Vector2 textureScale, Vector2 textureOffset, float textureRotation,
			 bool useSpecTex, const std::string& specTexName);

public:
	virtual ~Material();

	TiXmlElement*	ToXML() const;
	void			SaveToFile() const;

	static std::shared_ptr<Material>	CreateFromXML(TiXmlElement* element);
private:
	bool								LoadFromXML(TiXmlElement* element);
	void								CleanTexturesPaths();

public:
	static std::shared_ptr<Material>	GetDefault()
	{
		if (!_pDefaultMaterial) _pDefaultMaterial.reset(DBG_NEW Material);
		return _pDefaultMaterial;
	}
private:
	static std::shared_ptr<Material>	_pDefaultMaterial;

public:
	DECLARE_PROPERTY_READ_ONLY(std::string, Name);
	DECLARE_PROPERTY_READ_ONLY(std::string, FilePath);

	DECLARE_PROPERTY(Color, DiffuseColor);
	DECLARE_PROPERTY(Color, SpecularColor);
	DECLARE_PROPERTY(float, SpecularPower);
	DECLARE_PROPERTY(Color, AmbientColor);
	DECLARE_PROPERTY(bool, IsLit);
	
	DECLARE_PROPERTY(bool, IsTextured);
	DECLARE_PROPERTY(Vector2, TextureScale);
	DECLARE_PROPERTY(Vector2, TextureOffset);
	DECLARE_PROPERTY(float, TextureRotation);
	DECLARE_PROPERTY(std::string, TexturePath);
	DECLARE_PROPERTY(SamplingType, TextureSampler);
	
	DECLARE_PROPERTY(bool, UseSpecularTexture);
	DECLARE_PROPERTY(std::string, SpecularTexturePath);

	DECLARE_PROPERTY(bool, UseNormalMap);
	DECLARE_PROPERTY(std::string, NormalMapTexturePath);

	DECLARE_PROPERTY(bool, CastShadows);
	DECLARE_PROPERTY(bool, ReceiveShadows);

public:
	// TODO cache this info since texture parameters don't change that much
	Matrix	GetTextureMatrix() const
	{
		// this needs to be done so that the rotation occurs around the center point of the texture
		Matrix rotation = 
			Matrix::CreateTranslation(-0.5f, -0.5f, 0.0f) *
			Matrix::CreateRotationZ(DirectX::XMConvertToRadians(_TextureRotation)) * 
			Matrix::CreateTranslation(0.5f, 0.5f, 0.0f);
		return rotation * 
			Matrix::CreateScale(_TextureScale.x, _TextureScale.y, 1.0f) *
			Matrix::CreateTranslation(_TextureOffset.x, _TextureOffset.y, 0.0f);
	}

private:
	virtual bool HandleEvent(StrongEventDataPtr eventData);
	virtual void HandleRegistering(bool isRegistering);
};