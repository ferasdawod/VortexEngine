#pragma once

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
typedef wchar_t WCHAR;
struct ID3DX11Effect;

namespace D3DUtils
{
	bool CompileShaderFromFile(const WCHAR* fileName, const char* entryPoint, const char* shaderModel, ID3DBlob** blobOut);
	bool CreateEffectFromFile(const WCHAR* fileName, ID3DX11Effect** ppOutEffect);
}