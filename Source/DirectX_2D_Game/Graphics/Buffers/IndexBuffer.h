#pragma once

#include "IBuffer.h"
#include "Utilities/Macros.h"
enum DXGI_FORMAT;

class IndexBuffer : public IBuffer
{
	friend class GraphicsDevice;

public:
	// do not use new with this, use the create method instead
	IndexBuffer() = default;
	virtual ~IndexBuffer() = default;

	static std::shared_ptr<IndexBuffer>		Create(GraphicsDevice* device, WORD* indices, UINT numIndices, bool staticBuffer = true);

	DECLARE_PROPERTY_READ_ONLY(DXGI_FORMAT, Format);
	DECLARE_PROPERTY_READ_ONLY(bool, IsStatic);
	DECLARE_PROPERTY_READ_ONLY(UINT, NumIndices);
};

