#include "RenderTarget.h"

#include <d3d11.h>
#include <dxgi.h>

#include "GraphicsDevice.h"
#include "Debugging/Debug.h"

RenderTarget::RenderTarget() :
_DepthStencilShaderView(nullptr), _DepthStencilTexture(nullptr), _DepthStencilView(nullptr),
_RenderTargetShaderView(nullptr), _RenderTargetTexture(nullptr), _RenderTargetView(nullptr)
{
}

bool RenderTarget::Initialize(int width /*= -1*/, int height /*= -1*/)
{
	GraphicsDevice* gd = GraphicsDevice::GetPtr();

	_Width = width == -1 ? gd->BackBufferWidth() : width;
	_Height = height == -1 ? gd->BackBufferHeight() : height;

	// first create the depth stuff
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));

	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.Width = _Width;
	texDesc.Height = _Height;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	HRESULT hr = gd->GetDevice()->CreateTexture2D(&texDesc, nullptr, &_DepthStencilTexture);
	if (FAILED(hr))
	{
		LOG_E("Failed to create the depth texture", hr);
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));

	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.Texture2D.MipSlice = 0;

	hr = gd->GetDevice()->CreateDepthStencilView(_DepthStencilTexture.Get(), &depthDesc, &_DepthStencilView);
	if (FAILED(hr))
	{
		LOG_E("Failed to create the depth stencil view", hr);
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC depthShaderDesc;
	ZeroMemory(&depthShaderDesc, sizeof(depthShaderDesc));

	depthShaderDesc.Format = DXGI_FORMAT_R32_FLOAT;
	depthShaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	depthShaderDesc.Texture2D.MipLevels = 1;
	depthShaderDesc.Texture2D.MostDetailedMip = 0;

	hr = gd->GetDevice()->CreateShaderResourceView(_DepthStencilTexture.Get(), &depthShaderDesc, &_DepthStencilShaderView);
	if (FAILED(hr))
	{
		LOG_E("Failed to create the depth stencil shader view", hr);
		return false;
	}

	// render target initialization

	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Width = _Width;
	texDesc.Height = _Height;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = gd->GetDevice()->CreateTexture2D(&texDesc, nullptr, &_RenderTargetTexture);
	if (FAILED(hr))
	{
		LOG_E("Failed to create the render target texture", hr);
		return false;
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderDesc;
	ZeroMemory(&renderDesc, sizeof(renderDesc));

	renderDesc.Format = texDesc.Format;
	renderDesc.Texture2D.MipSlice = 0;
	renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	hr = gd->GetDevice()->CreateRenderTargetView(_RenderTargetTexture.Get(), &renderDesc, &_RenderTargetView);
	if (FAILED(hr))
	{
		LOG_E("Failed to create the render target view", hr);
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC renderShaderDesc;
	ZeroMemory(&renderShaderDesc, sizeof(renderShaderDesc));

	renderShaderDesc.Format = texDesc.Format;
	renderShaderDesc.Texture2D.MipLevels = 1;
	renderShaderDesc.Texture2D.MostDetailedMip = 0;
	renderShaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	hr = gd->GetDevice()->CreateShaderResourceView(_RenderTargetTexture.Get(), &renderShaderDesc, &_RenderTargetShaderView);
	if (FAILED(hr))
	{
		LOG_E("Failed to create the render target shader view", hr);
		return false;
	}

	return true;
}