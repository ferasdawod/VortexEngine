#pragma once

#include <memory>
#include "Utilities/Macros.h"

class VertexBuffer;
class IndexBuffer;
using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
using VertexIndexPair = std::pair<VertexBufferPtr, IndexBufferPtr>;
class Material;
enum D3D_PRIMITIVE_TOPOLOGY;
typedef D3D_PRIMITIVE_TOPOLOGY D3D11_PRIMITIVE_TOPOLOGY;

namespace DirectX
{
	struct BoundingBox;
	struct BoundingSphere;
}

using DirectX::BoundingBox;
using DirectX::BoundingSphere;

enum class BoundingShapeType
{
	Sphere = 0,
	Box,
};

class SubMesh
{
	friend class MeshLoader;
	friend class MeshRenderer;

private:
	SubMesh() = default;
	SubMesh(VertexIndexPair pair, D3D11_PRIMITIVE_TOPOLOGY topo) : 
		_VertexBuffer(pair.first),
		_IndexBuffer(pair.second),
		_Topology(topo),
		_BoundingShapeType(BoundingShapeType::Box)
	{
	}

public:
	virtual ~SubMesh() {}

	DECLARE_PROPERTY_READ_ONLY(std::shared_ptr<VertexBuffer>, VertexBuffer);
	DECLARE_PROPERTY_READ_ONLY(std::shared_ptr<IndexBuffer>, IndexBuffer);
	DECLARE_PROPERTY_READ_ONLY(D3D11_PRIMITIVE_TOPOLOGY, Topology);

	DECLARE_PROPERTY_READ_ONLY(BoundingShapeType, BoundingShapeType);
	DECLARE_PROPERTY_READ_ONLY(std::shared_ptr<BoundingBox>, BoundingBox);
	DECLARE_PROPERTY_READ_ONLY(std::shared_ptr<BoundingSphere>, BoundingSphere);
};