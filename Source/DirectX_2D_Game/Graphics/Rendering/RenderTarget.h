#pragma once

struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

#include "Utilities/Macros.h"
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

class RenderTarget
{
public:
	RenderTarget();
	virtual ~RenderTarget() = default;

	// initialize the render target
	// leave the width and height to -1 to set them to the back buffer width and height
	bool								Initialize(int width = -1, int height = -1);

	DECLARE_PROPERTY_READ_ONLY(ComPtr<ID3D11DepthStencilView>, DepthStencilView);
	DECLARE_PROPERTY_READ_ONLY(ComPtr<ID3D11Texture2D>, DepthStencilTexture);
	DECLARE_PROPERTY_READ_ONLY(ComPtr<ID3D11ShaderResourceView>, DepthStencilShaderView);

	DECLARE_PROPERTY_READ_ONLY(ComPtr<ID3D11RenderTargetView>, RenderTargetView);
	DECLARE_PROPERTY_READ_ONLY(ComPtr<ID3D11Texture2D>, RenderTargetTexture);
	DECLARE_PROPERTY_READ_ONLY(ComPtr<ID3D11ShaderResourceView>, RenderTargetShaderView);

	DECLARE_PROPERTY_READ_ONLY(int, Width);
	DECLARE_PROPERTY_READ_ONLY(int, Height);
};