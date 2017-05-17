#include "pch.h"

#include "Material.h"

#include "Colors.h"

std::shared_ptr<Material> Material::_pDefaultMaterial;

Material::Material()
: Material("Default", MyColors::White, MyColors::White, 32.0f, MyColors::White, true, false, "",
		SamplingType::LinearWrap, Vector2::One, Vector2::Zero, 0.0f, false, "")
{
}

Material::Material(const std::string& matName) : Material()
{
	_Name = matName;
}

Material::Material(const std::string& name, const Color& diffuseColor, const Color& specColor, float specPower,
				   const Color& ambientColor, bool isLit, bool isTextured, const std::string& textureName, SamplingType sameplingType,
				   Vector2 textureScale, Vector2 textureOffset, float textureRotation, bool useSpecularTexture, const std::string& specTexName)
				   : _Name(name), _DiffuseColor(diffuseColor), _SpecularColor(specColor), _SpecularPower(specPower),
				   _AmbientColor(ambientColor), _IsLit(isLit), _IsTextured(isTextured), _TexturePath(textureName),
				   _TextureSampler(sameplingType), _CastShadows(true), _ReceiveShadows(true), _TextureScale(textureScale),
				   _TextureOffset(textureOffset), _TextureRotation(textureRotation),
				   _UseSpecularTexture(useSpecularTexture), _SpecularTexturePath(specTexName),
				   _UseNormalMap(false), _NormalMapTexturePath("")
{
	HandleRegistering(true);
}

Material::~Material()
{
	if (this != _pDefaultMaterial.get())
		HandleRegistering(false);
}

TiXmlElement* Material::ToXML() const
{
	TiXmlElement* element = DBG_NEW TiXmlElement("Material");
	element->SetAttribute("Name", _Name);

	auto diffuseE = XmlHelper::ToXml("Diffuse", _DiffuseColor);
	auto ambientE = XmlHelper::ToXml("Ambient", _AmbientColor);
	auto specularE = XmlHelper::ToXml("Specular", _SpecularColor);

	auto specPowerE = XmlHelper::ToXml("SpecularPower", _SpecularPower);
	auto isLitE = XmlHelper::ToXml("IsLit", _IsLit);
	auto isTexE = XmlHelper::ToXml("IsTextured", _IsTextured);
	auto texName = XmlHelper::ToXml("TexturePath", _TexturePath);
	auto sampleTypeE = XmlHelper::ToXml("SamplingType", static_cast<int>(_TextureSampler));
	auto texScaleE = XmlHelper::ToXml("TextureScale", _TextureScale);
	auto texOffsetE = XmlHelper::ToXml("TextureOffset", _TextureOffset);
	auto texRotationE = XmlHelper::ToXml("TextureRotation", _TextureRotation);

	std::string sampleCommentMsg =
		std::string("Available Samplers : ") +
		"PointClamp = " + std::to_string(static_cast<int>(SamplingType::PointClamp)) + ", " + 
		"PointWrap = " + std::to_string(static_cast<int>(SamplingType::PointWrap)) + ", " +
		"LinearClamp = " + std::to_string(static_cast<int>(SamplingType::LinearClamp)) + ", " +
		"LinearWrap = " + std::to_string(static_cast<int>(SamplingType::LinearWrap)) + ", " +
		"AnisotropicClamp = " + std::to_string(static_cast<int>(SamplingType::AnisotropicClamp)) + ", " +
		"AnisotropicWrap = " + std::to_string(static_cast<int>(SamplingType::AnisotropicWrap));
	auto sampleCommnetE = DBG_NEW TiXmlComment(sampleCommentMsg.c_str());

	auto castShadowE = XmlHelper::ToXml("CastShadows", _CastShadows);
	auto receiveShadowE = XmlHelper::ToXml("ReceiveShadows", _ReceiveShadows);

	auto useSpecTexE = XmlHelper::ToXml("UseSpecularTexture", _UseSpecularTexture);
	auto specTexNameE = XmlHelper::ToXml("SpecularTexturePath", _SpecularTexturePath);

	auto useNorE = XmlHelper::ToXml("UseNormalMap", _UseNormalMap);
	auto norPathE = XmlHelper::ToXml("NormalMapTexturePath", _NormalMapTexturePath);


	element->LinkEndChild(diffuseE);
	element->LinkEndChild(ambientE);
	element->LinkEndChild(specularE);
	element->LinkEndChild(specPowerE);
	element->LinkEndChild(isLitE);
	element->LinkEndChild(isTexE);
	element->LinkEndChild(texName);
	element->LinkEndChild(sampleCommnetE);
	element->LinkEndChild(sampleTypeE);
	element->LinkEndChild(texScaleE);
	element->LinkEndChild(texOffsetE);
	element->LinkEndChild(texRotationE);
	element->LinkEndChild(useSpecTexE);
	element->LinkEndChild(specTexNameE);
	element->LinkEndChild(useNorE);
	element->LinkEndChild(norPathE);

	return element;
}

void Material::SaveToFile() const
{
	std::shared_ptr<TiXmlDocument> document(DBG_NEW TiXmlDocument());
	document->LinkEndChild(this->ToXML());

	std::string savePath = _FilePath.empty() ? "Assets\\Materials\\" + _Name + ".material" : _FilePath;
	document->SaveFile(savePath);
}

bool Material::LoadFromXML(TiXmlElement* element)
{
	// confirm that this element is indeed a material
	assert(element && element->ValueStr() == std::string("Material"));

	XmlHelper::FromXml(element, "Diffuse", _DiffuseColor);
	XmlHelper::FromXml(element, "Ambient", _AmbientColor);
	XmlHelper::FromXml(element, "Specular", _SpecularColor);
	XmlHelper::FromXml(element, "SpecularPower", _SpecularPower);
	XmlHelper::FromXml(element, "IsLit", _IsLit);

	XmlHelper::FromXml(element, "IsTextured", _IsTextured);
	XmlHelper::FromXml(element, "TexturePath", _TexturePath);
	
	XmlHelper::FromXml(element, "CastShadows", _CastShadows);
	XmlHelper::FromXml(element, "ReceiveShadows", _ReceiveShadows);

	XmlHelper::FromXml(element, "TextureOffset", _TextureOffset);
	XmlHelper::FromXml(element, "TextureScale", _TextureScale);
	XmlHelper::FromXml(element, "TextureRotation", _TextureRotation);

	XmlHelper::FromXml(element, "UseSpecularTexture", _UseSpecularTexture);
	XmlHelper::FromXml(element, "SpecularTexturePath", _SpecularTexturePath);

	XmlHelper::FromXml(element, "UseNormalMap", _UseNormalMap);
	XmlHelper::FromXml(element, "NormalMapTexturePath", _NormalMapTexturePath);

	element->QueryStringAttribute("Name", &_Name);

	int sampleInt;
	XmlHelper::FromXml(element, "SamplingType", sampleInt);
	_TextureSampler = static_cast<SamplingType>(sampleInt);

	return true;
}

std::shared_ptr<Material> Material::CreateFromXML(TiXmlElement* element)
{
	assert(element != nullptr && element->ValueStr() == std::string("Material"));
		
	string prefabPath;
	auto isPrefab = element->QueryStringAttribute("PrefabPath", &prefabPath);
	// if this actor reference a prefab then load it from the prefab 
	if (isPrefab == TIXML_SUCCESS)
	{
		TiXmlDocument materialDocument(prefabPath);
		if (materialDocument.LoadFile())
		{
			std::shared_ptr<Material> material(DBG_NEW Material);
			material->LoadFromXML(materialDocument.RootElement());
			material->_FilePath = prefabPath;

			return material;
		}

		return nullptr;
	}
	else
	{
		return GetDefault();
	}
}

bool Material::HandleEvent(StrongEventDataPtr eventData)
{
	if (eventData->GetID() == Event_ReloadMaterial::kEventID)
	{
		auto casted = std::dynamic_pointer_cast<Event_ReloadMaterial>(eventData);
		assert(casted);

		if (casted->MaterialName().empty() || casted->MaterialName() == _Name)
		{
			if (_FilePath.empty() == false)
			{
				TiXmlDocument materialDocument(_FilePath);
				if (materialDocument.LoadFile())
				{
					LoadFromXML(materialDocument.RootElement());
					return casted->MaterialName().empty() == false;
				}
			}
		}

		return false;
	}
	else
		assert(!"got an event that i didn't want");

	return false;
}

void Material::HandleRegistering(bool isRegistering)
{
	ToggleRegisteration(Event_ReloadMaterial::kEventID, isRegistering);
}