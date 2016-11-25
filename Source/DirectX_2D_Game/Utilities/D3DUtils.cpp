#include "D3DUtils.h"

#include "Debugging/Debug.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <FX11/d3dx11effect.h>
#include "Graphics/Rendering/GraphicsDevice.h"


bool D3DUtils::CompileShaderFromFile(const WCHAR* fileName, const char* entryPoint, const char* shaderModel, ID3DBlob** blobOut)
{
	HRESULT hr = S_OK;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(fileName, nullptr, nullptr, entryPoint, shaderModel, shaderFlags, 0, blobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			std::string message = "Failed to compile the shader, " + std::string(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			LOG_E(message.c_str(), hr);

			SAFE_RELEASE(pErrorBlob);
		}

		return false;
	}

	return true;
}

bool D3DUtils::CreateEffectFromFile(const WCHAR* fileName, ID3DX11Effect** ppOutEffect)
{
	// TODO
	// we should switch to column major for optimization aka transposing them before uploading to the GPU
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DX11CompileEffectFromFile(fileName, nullptr, nullptr, shaderFlags, 0,
											 GraphicsDevice::GetPtr()->GetDevice().Get(), ppOutEffect, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			std::string message = "Failed to create the effect, " + std::string(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
			LOG_E(message.c_str(), hr);

			SAFE_RELEASE(errorBlob);
		}

		return false;
	}

	return true;
}