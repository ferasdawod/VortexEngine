#pragma once

#include <memory>
#include <wrl/client.h>

class GraphicsDevice;
struct ID3D11Buffer;
using UINT = unsigned int;
using WORD = unsigned short;
using Microsoft::WRL::ComPtr;

class IBuffer
{
public:
	virtual ~IBuffer() {}

public:
	ComPtr<ID3D11Buffer>	GetRawBuffer() const { return _pBuffer; }

protected:
	ComPtr<ID3D11Buffer>	_pBuffer;
};