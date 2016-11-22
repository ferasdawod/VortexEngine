#pragma once

#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/PrimitiveBatch.h>
#include <memory>
#include "Graphics/Vertex.h"

using DirectX::SimpleMath::Vector3;
using DirectX::PrimitiveBatch;

class DebugRenderer
{
public:
	DebugRenderer();
	virtual ~DebugRenderer();

	void DrawLine(Vector3 p0, Vector3 p1);

protected:
	std::unique_ptr<PrimitiveBatch<Vertex>>	_pPrimitiveBatch;
};