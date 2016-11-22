#pragma once

#include "IBuffer.h"
#include "Utilities/Macros.h"

class VertexBuffer : public IBuffer
{
	friend class GraphicsDevice;

public:
	// do not use new with this, use the create method instead
	VertexBuffer() = default;
	virtual ~VertexBuffer() = default;

public:
	static std::shared_ptr<VertexBuffer>	Create(GraphicsDevice* device, void* vertices, UINT numVertices, UINT vertexSize, bool staticBuffer = true);

	DECLARE_PROPERTY_READ_ONLY(UINT, Stride);
	DECLARE_PROPERTY_READ_ONLY(UINT, Offset);
	DECLARE_PROPERTY_READ_ONLY(bool, IsStatic);
	DECLARE_PROPERTY_READ_ONLY(UINT, NumVertices);
};