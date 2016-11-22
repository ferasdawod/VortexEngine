#pragma once

#include <memory>
#include <string>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

#include "Resources/Resource.h"
#include "Utilities/Macros.h"

struct ID3D11ShaderResourceView;

class Texture2D : public Resource
{
	friend class Texture2DLoader;

private:
	Texture2D(const std::string& resourceName, ComPtr<ID3D11ShaderResourceView> shaderView) :
		Resource(resourceName),
		_ShaderResourceView(shaderView)
	{
	}

public:
	virtual ~Texture2D() {}

	DECLARE_PROPERTY_READ_ONLY(ComPtr<ID3D11ShaderResourceView>, ShaderResourceView);
};