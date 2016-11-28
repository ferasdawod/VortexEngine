#pragma once

#include <memory>
#include <wrl/client.h>
#include <d3d11.h>
#include "Graphics/Colors.h"

#include "Events/EventListener.h"
#include "Utilities/Singleton.h"
#include "GraphicsEnums.h"

class ViewPort;
class VertexBuffer;
class IndexBuffer;
class RenderTarget;
class SubMesh;

namespace DirectX
{
	class CommonStates;
}
using DirectX::CommonStates;
using Microsoft::WRL::ComPtr;


class GraphicsDevice : public EventListener, public Singleton<GraphicsDevice>
{
public:
	explicit						GraphicsDevice();
									~GraphicsDevice();

public:
	bool				Initialize(HWND hWnd);
	void				Release();

	void				Clear();
	void				Clear(std::shared_ptr<RenderTarget> renderTarget);

	void				Draw(std::shared_ptr<SubMesh> subMesh);
	void				Present();

	void				SetFullScreenState(bool state);
	void				SetDefaultTargets();
	// set the provided render target as the current target to be rendered to.
	// to reset the render target call this method with null parameter or call "SetDefaultTargets()"
	void				SetRenderTarget(std::shared_ptr<RenderTarget> renderTarget);

	void				SetDefaultStates();
	void				SetDepthState(DepthState state);
	void				SetCullState(CullState state);
	void				SetAlphaState(AlphaState state);
	void				SetViewPort(const ViewPort& viewPort, bool absoluteValues = false);
	ComPtr<ID3D11SamplerState>	GetSampler(SamplingType type) const;

public:
	bool				OnResize(int newWidth, int newHeight);

	virtual bool		HandleEvent(StrongEventDataPtr eventData);

	D3D_FEATURE_LEVEL		GetFeatureLevel() const { return _featureLevel; }
	D3D_DRIVER_TYPE			GetDriverType() const { return _driverType; }

	UINT					BackBufferWidth() const { return _nBackBufferWidth; }
	UINT					BackBufferHeight() const { return _nBackBufferHeight; }

private:
	void					SetVertexBuffer(std::shared_ptr<VertexBuffer> buffer);
	void					SetIndexBuffer(std::shared_ptr<IndexBuffer> buffer);
	virtual void			HandleRegistering(bool isRegistering);
	void					LoadSettings();

public:
	ComPtr<ID3D11Device>			GetDevice() const { return _pDevice; }
	ComPtr<ID3D11DeviceContext>		GetContext() const { return _pContext; }

private:
	ComPtr<ID3D11Device>						_pDevice;
	ComPtr<ID3D11DeviceContext>					_pContext;
	ComPtr<IDXGISwapChain>						_pSwapChain;

	ComPtr<ID3D11RenderTargetView>				_pRenderTargetView;
	ComPtr<ID3D11DepthStencilView>				_pDepthStencilView;

	std::shared_ptr<CommonStates>				_pCommonStates;

	D3D_FEATURE_LEVEL							_featureLevel;
	D3D_DRIVER_TYPE								_driverType;

	HWND										_hWnd;
	UINT										_nBackBufferWidth;
	UINT										_nBackBufferHeight;

	Color										_clearColor;
	int											_nVSyncCount;
};