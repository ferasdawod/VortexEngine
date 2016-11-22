#include "ShadowMap.h"

#include "GraphicsDevice.h"
#include "Debugging/Debug.h"
#include <DirectXTK/DirectXHelpers.h>

// update the define in the shader to reflect this
#define SMAP_SIZE 2048

ShadowMap::ShadowMap(int size /*= -1*/)
{
	if (size == -1)
		_nSize = SMAP_SIZE;
	else
		_nSize = size;

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));

	texDesc.Width = _nSize;
	texDesc.Height = _nSize;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* depthMap = nullptr;
	HRESULT hr = GraphicsDevice::GetPtr()->GetDevice()->CreateTexture2D(&texDesc, nullptr, &depthMap);
	if (FAILED(hr))
		LOG_E("Failed to create the depth texture", hr);

	D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));

	viewDesc.Flags = 0;
	viewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice = 0;

	hr = GraphicsDevice::GetPtr()->GetDevice()->CreateDepthStencilView(depthMap, &viewDesc, &_DepthStencilView);
	if (FAILED(hr))
		LOG_E("Failed to create the depth stencil view", 0);

	DirectX::SetDebugObjectName(_DepthStencilView.Get(), "ShadowMapDepthStencilView");

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc;
	ZeroMemory(&shaderDesc, sizeof(shaderDesc));

	shaderDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderDesc.Texture2D.MipLevels = texDesc.MipLevels;
	shaderDesc.Texture2D.MostDetailedMip = 0;

	hr = GraphicsDevice::GetPtr()->GetDevice()->CreateShaderResourceView(depthMap, &shaderDesc, &_ShaderResourceView);
	if (FAILED(hr))
		LOG_E("Failed to create the depth shader resource view", hr);

	DirectX::SetDebugObjectName(_ShaderResourceView.Get(), "ShadowMapShaderResourceView");

	SAFE_RELEASE(depthMap);
}

void ShadowMap::SetAsActiveTarget()
{
	auto context = GraphicsDevice::GetPtr()->GetContext();

	const D3D11_VIEWPORT& viewport = GetViewPort();
	context->RSSetViewports(1, &viewport);

	ID3D11RenderTargetView* renderTargets[1] = { 0 };
	context->OMSetRenderTargets(0, renderTargets, _DepthStencilView.Get());
	context->ClearDepthStencilView(_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

const D3D11_VIEWPORT& ShadowMap::GetViewPort() const
{
	static D3D11_VIEWPORT viewPort{ 0 };
	static bool initialized{ false };
	if (!initialized)
	{
		viewPort.Height = viewPort.Width = static_cast<FLOAT>(_nSize);
		viewPort.TopLeftX = viewPort.TopLeftY = 0.0f;
		viewPort.MaxDepth = 1.0f;
		viewPort.MinDepth = 0.0f;

		initialized = true;
	}

	return viewPort;
}
