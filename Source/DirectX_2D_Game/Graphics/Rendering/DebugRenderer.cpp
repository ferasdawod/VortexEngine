#include "DebugRenderer.h"

#include "Debugging/Debug.h"
#include "GraphicsDevice.h"

DebugRenderer::DebugRenderer()
{
	_pPrimitiveBatch.reset(DBG_NEW PrimitiveBatch<Vertex>(GraphicsDevice::GetPtr()->GetContext().Get()));
}

DebugRenderer::~DebugRenderer()
{
}


void DebugRenderer::DrawLine(Vector3 p0, Vector3 p1)
{
	_pPrimitiveBatch->Begin();
	_pPrimitiveBatch->DrawLine(Vertex(p0), Vertex(p1));
	_pPrimitiveBatch->End();
}
