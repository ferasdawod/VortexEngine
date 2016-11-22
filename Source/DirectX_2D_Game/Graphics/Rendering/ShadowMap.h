#pragma once

#include "Utilities/Macros.h"
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;
struct D3D11_VIEWPORT;

class ShadowMap
{
public:
	ShadowMap(int size = -1);
	virtual ~ShadowMap() {}

	void									SetAsActiveTarget();

	DECLARE_PROPERTY_READ_ONLY(ComPtr<ID3D11DepthStencilView>, DepthStencilView);
	DECLARE_PROPERTY_READ_ONLY(ComPtr<ID3D11ShaderResourceView>, ShaderResourceView);

public:
	const D3D11_VIEWPORT&					GetViewPort() const;
private:
	int				_nSize;
};