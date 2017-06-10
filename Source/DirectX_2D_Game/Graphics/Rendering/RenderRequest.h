#pragma once

#include <DirectXTK/SimpleMath.h>
#include <memory>
#include <vector>
#include "Utilities/Macros.h"

class Material;
class Transform;
class IComponent;

using WeakRenderRequestPtr = std::weak_ptr<RenderRequest>;
using StrongRenderRequestPtr = std::shared_ptr<RenderRequest>;

using MaterialsVector = std::vector<std::shared_ptr<Material>>;

class RenderRequest
{
public:
	RenderRequest() : _Materials(nullptr) {}
	virtual ~RenderRequest() = default;

	void						SetMaterials(const MaterialsVector& materials) { _Materials = &materials; }
	//void						AddMaterial(std::shared_ptr<Material> mat) { assert(mat); _Materials.push_back(mat); }
	size_t						GetMaterialsCount() const { return _Materials != nullptr ? _Materials->size() : 0; }
	std::shared_ptr<Material>	GetMaterial(size_t index)
	{
		if (_Materials == nullptr) return std::shared_ptr<Material>();

		assert(index < _Materials->size());

		return (*_Materials)[index];
	}

	DECLARE_STRING_PROPERTY(MeshName);
	DECLARE_PROPERTY(std::weak_ptr<Transform>, Transform);
	DECLARE_PROPERTY(std::weak_ptr<IComponent>, Owner);

private:
	const MaterialsVector*		_Materials;
};