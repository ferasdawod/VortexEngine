#pragma once

#include "BaseComponent.h"
#include <DirectXCollision.h>
#include "Utilities/Macros.h"

#include <string>
#include <memory>
#include <vector>

class Mesh;
class Material;
class RenderRequest;
using MaterialPtr = std::shared_ptr<Material>;
using MaterialsVector = std::vector<MaterialPtr>;
using RenderRequestPtr = std::shared_ptr<RenderRequest>;
using RenderRequestsVector = std::vector<RenderRequestPtr>;

class MeshRenderer : public BaseComponent, public std::enable_shared_from_this<MeshRenderer>
{
public:
	static const ComponentTypeId	kComponentID;

public:
	MeshRenderer(const std::string& meshName = "");
	virtual ~MeshRenderer() {}

	EXPOSE_ID(kComponentID);

	virtual void Initialize();
	virtual void Initialize(TiXmlElement* xmlData);

	virtual void OnUpdate(float deltaTime) override { }

	void RegisterProperties() override;
	void RemoveMaterial(ObjectId materialId);

	virtual TiXmlElement* ToXML() const override;

	const std::string& GetMeshFilePath() const { return _MeshFilePath; }
	void SetMeshFilePath(const std::string& meshFilePath);

	DECLARE_PROPERTY_READ_ONLY(RenderRequestPtr, RenderRequest);
	DECLARE_PROPERTY_READ_ONLY(MaterialsVector, Materials);

	void AddMaterial(std::shared_ptr<Material> material);

protected:
	std::string _MeshFilePath;
};

__declspec(selectany) const ComponentTypeId MeshRenderer::kComponentID = 0x6B7EA484;