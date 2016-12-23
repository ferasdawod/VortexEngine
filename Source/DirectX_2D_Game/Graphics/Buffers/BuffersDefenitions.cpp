#include "pch.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Graphics/Rendering/GraphicsDevice.h"

std::shared_ptr<VertexBuffer> VertexBuffer::Create(GraphicsDevice* device, void* vertices, UINT numVertices, UINT vertexSize, bool staticBuffer /* = true */)
{
	std::shared_ptr<VertexBuffer> buffer(DBG_NEW VertexBuffer());

	D3D11_BUFFER_DESC buffDesc = { 0 };
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.ByteWidth = vertexSize * numVertices;
	buffDesc.CPUAccessFlags = staticBuffer ? 0 : D3D11_CPU_ACCESS_WRITE;
	buffDesc.Usage = staticBuffer ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = vertices;

	auto hr = device->GetDevice()->CreateBuffer(&buffDesc, &data, &buffer->_pBuffer);
	if (FAILED(hr))
	{
		LOG_E("Failed to create a vertex buffer", hr);
		return nullptr;
	}

	buffer->_IsStatic = staticBuffer;
	buffer->_NumVertices = numVertices;
	buffer->_Offset = 0;
	buffer->_Stride = vertexSize;

	return buffer;
}

std::shared_ptr<IndexBuffer> IndexBuffer::Create(GraphicsDevice* device, WORD* indices, UINT numIndices, bool staticBuffer /* = true */)
{
	auto buffer = std::make_shared<IndexBuffer>();

	D3D11_BUFFER_DESC buffDesc = { 0 };
	buffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffDesc.ByteWidth = sizeof(WORD)* numIndices;
	buffDesc.CPUAccessFlags = staticBuffer ? 0 : D3D11_CPU_ACCESS_WRITE;
	buffDesc.Usage = staticBuffer ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = indices;

	auto hr = device->GetDevice()->CreateBuffer(&buffDesc, &data, &buffer->_pBuffer);
	if (FAILED(hr))
	{
		LOG_E("Failed to create an index buffer", hr);
		return nullptr;
	}

	buffer->_IsStatic = staticBuffer;
	buffer->_Format = DXGI_FORMAT_R16_UINT;
	buffer->_NumIndices = numIndices;

	return buffer;
}