#pragma once
#include "../Hook.h"
#include "../kiero/kiero.h"
#include <d3d11.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "../../../../include/ImFX/imfx.h"

class DirectX12 {
public:
	struct BackBufferContext {
		ID3D12Resource* resource;
		D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle;
	};

	static inline HWND window = nullptr;
	static inline bool initialized = false;
	static inline unsigned int bufferCount = 0;
	static inline bool skipped = false;
	static inline int ticks = 0;

	static inline ID3D12Device* device = nullptr;
	static inline BackBufferContext* contexts = nullptr;
	static inline ID3D12CommandAllocator** allocators;
	static inline ID3D12GraphicsCommandList* commandList = nullptr;
	static inline ID3D12CommandQueue* commandQueue = nullptr;
	static inline ID3D12DescriptorHeap* descriptorHeapImGuiRender = nullptr;
	static inline ID3D12DescriptorHeap* descriptorHeapBackBuffers = nullptr;

	typedef HRESULT(__fastcall* OriginalExecute)(ID3D12CommandQueue*, UINT, ID3D12CommandList*);
	static inline OriginalExecute oExecute = nullptr;
	typedef HRESULT(__fastcall* OriginalResize)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
	static inline OriginalResize oResize = nullptr;

	typedef HRESULT(__fastcall* OriginalDrawInstanced)(ID3D12GraphicsCommandList*, UINT, UINT, UINT, UINT);
	static inline OriginalDrawInstanced oDrawInstanced = nullptr;
	typedef HRESULT(__fastcall* OriginalDrawIndexed)(ID3D12GraphicsCommandList*, UINT, UINT, UINT, INT);
	static inline OriginalDrawIndexed oDrawIndexedInstance = nullptr;

	static void onPresent(IDXGISwapChain3*, UINT, UINT);
	static HRESULT onExecuteCommandList(ID3D12CommandQueue*, UINT, ID3D12CommandList*);
	static HRESULT onResizeBuffers(IDXGISwapChain3*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
	static HRESULT onDrawInstanced(ID3D12GraphicsCommandList*, UINT, UINT, UINT, UINT);
	static HRESULT onDrawIndexedInstance(ID3D12GraphicsCommandList*, UINT, UINT, UINT, INT);

	static bool setup(IDXGISwapChain3* swapChain);
	static void createRTV(IDXGISwapChain3*);
	static void releaseRTV();
};

class DirectX11 {
public:
	static inline HWND window = nullptr;
	static inline bool initialized = false;

	static inline ID3D11Device* device = nullptr;
	static inline ID3D11DeviceContext* context = nullptr;
	static inline ID3D11RenderTargetView* targetView = nullptr;

	typedef HRESULT(__fastcall* OriginResizeBuffer)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
	static inline OriginResizeBuffer oResizeBuffers = nullptr;

	static void onPresent(IDXGISwapChain*, UINT, UINT);
	static HRESULT  onResizeBuffers(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
};

class DirectXHook {
public:
	typedef HRESULT(__fastcall* OriginalPresent)(IDXGISwapChain3*, UINT, UINT);
	static inline OriginalPresent oPresent = nullptr;

	static HRESULT onPresent(IDXGISwapChain3*, UINT, UINT);
	static void SetupImGui();
	static void onRender2D();

	static void init();
	static void shutdown();
};

