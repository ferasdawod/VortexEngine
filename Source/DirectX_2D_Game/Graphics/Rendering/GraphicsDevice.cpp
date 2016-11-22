#include "GraphicsDevice.h"

#include <DirectXTK/CommonStates.h>
#include <DirectXTK/DirectXHelpers.h>

#include "Debugging/Debug.h"

#include "Graphics/Buffers/VertexBuffer.h"
#include "Graphics/Buffers/IndexBuffer.h"
#include "Graphics/SubMesh.h"
#include "Graphics/Colors.h"
#include "RenderTarget.h"

#include "Events/EventManager.h"
#include "Events/EventsTypes.h"

#include "Application/SettingsManager.h"

#include "3rd Party/TinyXml/tinyxml.h"
#include "Utilities/Utils.h"
#include "Utilities/XmlHelper.h"

GraphicsDevice::GraphicsDevice() :
_pDevice(nullptr),
_pContext(nullptr),
_pSwapChain(nullptr),
_pRenderTargetView(nullptr),
_pDepthStencilView(nullptr),
_featureLevel(D3D_FEATURE_LEVEL_9_1),
_clearColor(MyColors::Magenta),
_nVSyncCount(1)
{
	GraphicsDevice::HandleRegistering(true);
}

GraphicsDevice::~GraphicsDevice()
{
	GraphicsDevice::HandleRegistering(false);
	Release();
}
 
bool GraphicsDevice::Initialize(HWND hWnd)
{
	_hWnd = hWnd;

	Release();

	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	_nBackBufferWidth = width;
	_nBackBufferHeight = height;

	UINT deviceFlags = 0;
	deviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined(_DEBUG) || defined(_DEBUG)
	// TODO restore this after downloading the windows 10 sdk
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width = 0; // can be inferred from the window handle
	swapChainDesc.BufferDesc.Height = 0; // can be inferred from the window handle
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = true;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = S_OK;

	for (UINT i = 0; i < numDriverTypes; ++i)
	{
		_driverType = driverTypes[i];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, deviceFlags,
												featureLevels, numFeatureLevels, D3D11_SDK_VERSION,
												&swapChainDesc, _pSwapChain.GetAddressOf(), _pDevice.GetAddressOf(), &_featureLevel, _pContext.GetAddressOf());

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
	{
		LOG_E("Failed to create the directx device", hr);
		return false;
	}

	std::string deviceName = "MyDirect3D11Device";
	_pDevice->SetPrivateData(WKPDID_D3DDebugObjectName, deviceName.length(), deviceName.c_str());
	deviceName = "MyDirect3D11SwapChain";
	_pSwapChain->SetPrivateData(WKPDID_D3DDebugObjectName, deviceName.length(), deviceName.c_str());
	DirectX::SetDebugObjectName(_pContext.Get(), "MyDirect3D11Context");

	_pCommonStates.reset(DBG_NEW CommonStates(_pDevice.Get()));

	OnResize(width, height);
	LoadSettings();
	SetDefaultStates();
	SetDefaultTargets();
	
	return true;
}

bool GraphicsDevice::OnResize(int newWidth, int newHeight)
{
	LOG_M("Resizing");
	_pContext->OMSetRenderTargets(0, nullptr, nullptr);

	assert(_pRenderTargetView.Reset() == 0);
	assert(_pDepthStencilView.Reset() == 0);

	_pSwapChain->ResizeBuffers(1, newWidth, newHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	ID3D11Texture2D* backBufferTex = nullptr;
	auto hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTex);
	if (FAILED(hr))
	{
		LOG_E("Failed to get the back buffer texture", hr);
		return false;
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderDesc;
	ZeroMemory(&renderDesc, sizeof(renderDesc));

	renderDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderDesc.Texture2D.MipSlice = 0;
	renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	hr = _pDevice->CreateRenderTargetView(backBufferTex, &renderDesc, _pRenderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_E("Failed to create the render target view", hr);
		return false;
	}
	SAFE_RELEASE(backBufferTex);

	DirectX::SetDebugObjectName(_pRenderTargetView.Get(), "MyDirect3DRenderTargetView");

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.Width = newWidth;
	textureDesc.Height = newHeight;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	
	ID3D11Texture2D* depthTexture = nullptr;
	hr = _pDevice->CreateTexture2D(&textureDesc, nullptr, &depthTexture);
	if (FAILED(hr))
	{
		LOG_E("Failed to create the depth texture", hr);
		return false;
	}
	DirectX::SetDebugObjectName(depthTexture, "MyDirect3DDepthTexture");

	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.Format = textureDesc.Format;
	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthDesc.Texture2D.MipSlice = 0;

	hr = _pDevice->CreateDepthStencilView(depthTexture, &depthDesc, _pDepthStencilView.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_E("Failed to create the depth view", hr);
		return false;
	}
	SAFE_RELEASE(depthTexture);
	DirectX::SetDebugObjectName(_pDepthStencilView.Get(), "MyDirect3DDepthStencilView");

	_nBackBufferHeight = newHeight;
	_nBackBufferWidth = newWidth;

	return true;
}

void GraphicsDevice::SetDefaultTargets()
{
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(_nBackBufferWidth);
	vp.Height = static_cast<FLOAT>(_nBackBufferHeight);
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = vp.TopLeftY = 0.0f;
	
	_pContext->RSSetViewports(1, &vp);
	_pContext->OMSetRenderTargets(1, _pRenderTargetView.GetAddressOf(), _pDepthStencilView.Get());
}

void GraphicsDevice::SetDefaultStates()
{
	_pContext->RSSetState(_pCommonStates->CullCounterClockwise());
	_pContext->OMSetDepthStencilState(_pCommonStates->DepthDefault(), 0);
	_pContext->OMSetBlendState(_pCommonStates->Opaque(), MyColors::Black, 0xFFFFFFFF);
}

ComPtr<ID3D11SamplerState> GraphicsDevice::GetSampler(SamplingType type) const
{
	ID3D11SamplerState* sampler = nullptr;

	switch (type)
	{
		case SamplingType::AnisotropicClamp:
			sampler = _pCommonStates->AnisotropicClamp();
			break;
		case SamplingType::AnisotropicWrap:
			sampler = _pCommonStates->AnisotropicWrap();
			break;
		case SamplingType::LinearClamp:
			sampler = _pCommonStates->LinearClamp();
			break;
		case SamplingType::LinearWrap:
			sampler = _pCommonStates->LinearWrap();
			break;
		case SamplingType::PointClamp:
			sampler = _pCommonStates->PointClamp();
			break;
		case SamplingType::PointWrap:
			sampler = _pCommonStates->PointWrap();
			break;
	}

	return ComPtr<ID3D11SamplerState>(sampler);
}

void GraphicsDevice::SetDepthState(DepthState state)
{
	ID3D11DepthStencilState* Depthstate = nullptr;

	switch (state)
	{
		case DepthState::Default:
			Depthstate = _pCommonStates->DepthDefault();
			break;
		case DepthState::Read:
			Depthstate = _pCommonStates->DepthRead();
			break;
		case DepthState::None:
			Depthstate = _pCommonStates->DepthNone();
			break;
	}

	if (Depthstate != nullptr)
		_pContext->OMSetDepthStencilState(Depthstate, 0);
}

void GraphicsDevice::SetCullState(CullState state)
{
	ID3D11RasterizerState* rastState = nullptr;

	switch (state)
	{
		case CullState::None:
			rastState = _pCommonStates->CullNone();
			break;
		case CullState::ClockWise:
			rastState = _pCommonStates->CullClockwise();
			break;
		case CullState::CounterClockWise:
			rastState = _pCommonStates->CullCounterClockwise();
			break;
		case CullState::Wireframe:
			rastState = _pCommonStates->Wireframe();
			break;
	}

	if (rastState != nullptr)
		_pContext->RSSetState(rastState);
}

void GraphicsDevice::SetAlphaState(AlphaState state)
{
	ID3D11BlendState* blendState = nullptr;

	switch (state)
	{
		case AlphaState::AlphaBlend:
			blendState = _pCommonStates->AlphaBlend();
			break;
		case AlphaState::Additive:
			blendState = _pCommonStates->Additive();
			break;
		case AlphaState::NonPremultiplied:
			blendState = _pCommonStates->NonPremultiplied();
			break;
		case AlphaState::Opaque:
			blendState = _pCommonStates->Opaque();
			break;
	}

	if (blendState != nullptr)
		_pContext->OMSetBlendState(blendState, MyColors::Black, 0xFFFFFFFF);
}

void GraphicsDevice::Release()
{
#if defined(DEBUG) || defined(_DEBUG)
	if (_pDevice)
	{
		ID3D11Debug* _pDebug = nullptr;
		HRESULT hr = _pDevice.Get()->QueryInterface(__uuidof(ID3D11Debug), (void**)&_pDebug);
		if (SUCCEEDED(hr))
		{
			LOG_M("Begin Dump Live Objects");
			_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			LOG_M("\nFinish Dump Live Objects");

			SAFE_RELEASE(_pDebug);
		}
	}

	if (_pSwapChain)
	{
		UINT lastPresentCount = 0;
		HRESULT hr = _pSwapChain->GetLastPresentCount(&lastPresentCount);
		if (SUCCEEDED(hr))
		{
			LOG_M("Swap Chain Present Count = " + std::to_string(lastPresentCount));
		}
	}
#endif

	if (_pSwapChain)
	{
		_pSwapChain->SetFullscreenState(FALSE, nullptr);
	}
}

void GraphicsDevice::Clear()
{
	_pContext->ClearRenderTargetView(_pRenderTargetView.Get(), _clearColor);
	_pContext->ClearDepthStencilView(_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicsDevice::Clear(std::shared_ptr<RenderTarget> renderTarget)
{
	assert(renderTarget);

	if (renderTarget->GetRenderTargetView())
		_pContext->ClearRenderTargetView(renderTarget->GetRenderTargetView().Get(), _clearColor);
	if (renderTarget->GetDepthStencilView())
		_pContext->ClearDepthStencilView(renderTarget->GetDepthStencilView().Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicsDevice::Present()
{
	HRESULT hr = _pSwapChain->Present(_nVSyncCount, 0);
	if (FAILED(hr))
		LOG_E("Failed to present", hr);
}

void GraphicsDevice::SetVertexBuffer(std::shared_ptr<VertexBuffer> buffer)
{
	_pContext->IASetVertexBuffers(0, 1, buffer->_pBuffer.GetAddressOf(), &buffer->_Stride, &buffer->_Offset);
}

void GraphicsDevice::SetIndexBuffer(std::shared_ptr<IndexBuffer> buffer)
{
	_pContext->IASetIndexBuffer(buffer->_pBuffer.Get(), buffer->_Format, 0);
}

void GraphicsDevice::Draw(std::shared_ptr<SubMesh> subMesh)
{
	_pContext->IASetPrimitiveTopology(subMesh->GetTopology());

	auto vertBuff = subMesh->GetVertexBuffer();
	auto indexBuff = subMesh->GetIndexBuffer();

	if (!vertBuff)
		LOG_E("Attempting to draw with invalid vertex buffer", 0);

	if (!indexBuff)
	{
		SetVertexBuffer(vertBuff);
		_pContext->Draw(vertBuff->GetNumVertices(), 0);
	}
	else
	{
		SetIndexBuffer(indexBuff);
		SetVertexBuffer(vertBuff);
		_pContext->DrawIndexed(indexBuff->GetNumIndices(), 0, 0);
	}
}

bool GraphicsDevice::HandleEvent(StrongEventDataPtr eventData)
{
	if (eventData->GetID() == Event_WindowResized::kEventID)
	{
		auto casted = std::dynamic_pointer_cast<Event_WindowResized>(eventData);
		assert(casted != nullptr);

		OnResize(casted->Width(), casted->Height());
	}
	else if (eventData->GetID() == Event_SavingSettings::kEventID)
	{
		auto casted = std::dynamic_pointer_cast<Event_SavingSettings>(eventData);
		assert(casted != nullptr);

		auto settingsManager = casted->GetSettingsManager().lock();
		assert(settingsManager);

		settingsManager->SaveSetting("Graphics", "VSyncCount", _nVSyncCount);
		settingsManager->SaveSetting("Graphics", "ClearColor", _clearColor);
	}
	else
		assert(false);		// we should not get any more events

	return false;
}

void GraphicsDevice::HandleRegistering(bool isRegistering)
{
	ToggleRegisteration(Event_WindowResized::kEventID, isRegistering);
	ToggleRegisteration(Event_SavingSettings::kEventID, isRegistering);
}

void GraphicsDevice::SetRenderTarget(std::shared_ptr<RenderTarget> renderTarget)
{
	if (!renderTarget)
	{
		SetDefaultTargets();
		return;
	}
	
	if (renderTarget->GetWidth() != _nBackBufferWidth || renderTarget->GetHeight() != _nBackBufferHeight)
	{
		D3D11_VIEWPORT vp;
		vp.Width = static_cast<FLOAT>(renderTarget->GetWidth());
		vp.Height = static_cast<FLOAT>(renderTarget->GetHeight());
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = vp.TopLeftY = 0;

		_pContext->RSSetViewports(1, &vp);
	}

	auto renderTargetView = renderTarget->GetRenderTargetView();
	_pContext->OMSetRenderTargets(1, renderTargetView == nullptr ? nullptr : renderTargetView.GetAddressOf(),
								  renderTarget->GetDepthStencilView().Get());
}

void GraphicsDevice::LoadSettings()
{
	SettingsManager::GetPtr()->GetSetting("Graphics", "VSyncCount", _nVSyncCount);
	SettingsManager::GetPtr()->GetSetting("Graphics", "ClearColor", _clearColor);
}

void GraphicsDevice::SetFullScreenState(bool state)
{
	if (state)
		_pSwapChain->SetFullscreenState(TRUE, nullptr);
	else
		_pSwapChain->SetFullscreenState(FALSE, nullptr);
}
