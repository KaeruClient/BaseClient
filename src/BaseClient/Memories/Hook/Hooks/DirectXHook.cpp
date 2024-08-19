#include "DirectXHook.h"
#include "../../../../include/ImGui/imgui.h"
#include "../../../../include/ImGui/imgui_impl_dx11.h"
#include "../../../../include/ImGui/imgui_impl_win32.h"
#include "../../../../include/ImGui/imgui_impl_dx12.h"

#include "../../../../include/Fonts/Mojangles.h"


void DirectXHook::onRender2D() {
	GuiData* guiData = GameData::getGuiData();
	ImDrawList* d = ImGui::GetBackgroundDrawList();
	d->AddRectFilled(ImVec2(0.f, 0.f), ImVec2(60.f, 60.f), ImColor(255,255,255));
	ImFX::Begin(d);
	ImFX::AddBlur(5.f, ImVec4(0.f, 0.f, guiData->widthReal, guiData->heightReal));
	ImFX::End();
}


void DirectX12::onPresent(IDXGISwapChain3* swapChain, UINT syncInterval, UINT flags) {

	if (!commandQueue)
		return;

	if (!initialized) {
		if (setup(swapChain))
			initialized = true;
		return;
	}

	GuiData* guiData = GameData::getGuiData();
	if (guiData != nullptr) {
		createRTV(swapChain);

		ImVec2 windowSize = ImVec2(guiData->widthReal, guiData->heightReal);
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame(windowSize);

		ImGui::NewFrame();
		DirectXHook::onRender2D();
		ImGui::EndFrame();

		BackBufferContext currentContext = contexts[swapChain->GetCurrentBackBufferIndex()];
		ID3D12CommandAllocator* allocator = allocators[swapChain->GetCurrentBackBufferIndex()];
		allocator->Reset();

		D3D12_RESOURCE_BARRIER barrier;
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = currentContext.resource;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		commandList->Reset(allocator, nullptr);
		commandList->ResourceBarrier(1, &barrier);
		commandList->OMSetRenderTargets(1, &currentContext.descriptorHandle, FALSE, nullptr);
		ID3D12DescriptorHeap* ppDescriptors[] = { descriptorHeapImGuiRender };
		commandList->SetDescriptorHeaps(1, ppDescriptors);

		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
		//ImFX::EndFrame();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		commandList->ResourceBarrier(1, &barrier);
		commandList->Close();

		ID3D12CommandList* ppCommandLists[] = { commandList };
		commandQueue->ExecuteCommandLists(1, ppCommandLists);

		releaseRTV();
	}
	return;
}

HRESULT DirectX12::onExecuteCommandList(ID3D12CommandQueue* queue, UINT num, ID3D12CommandList* list) {
	commandQueue = queue;
	return oExecute(queue, num, list);
}

HRESULT DirectX12::onResizeBuffers(IDXGISwapChain3* a1, UINT a2, UINT a3, UINT a4, DXGI_FORMAT a5, UINT a6) {
	ticks = 5;
	releaseRTV();
	HRESULT result = oResize(a1, a2, a3, a4, a5, a6);
	createRTV(a1);
	return result;
}

HRESULT DirectX12::onDrawInstanced(ID3D12GraphicsCommandList* a1, UINT a2, UINT a3, UINT a4, UINT a5) {
	return oDrawInstanced(a1, a2, a3, a4, a5);
}

HRESULT DirectX12::onDrawIndexedInstance(ID3D12GraphicsCommandList* a1, UINT a2, UINT a3, UINT a4, INT a5) {
	return oDrawIndexedInstance(a1, a2, a3, a4, a5);
}

bool DirectX12::setup(IDXGISwapChain3* swapChain) {
	if (SUCCEEDED(swapChain->GetDevice(IID_PPV_ARGS(&device)))) {
		window = (HWND)FindWindowA(nullptr, (LPCSTR)"Minecraft");;

		DirectXHook::SetupImGui();

		DXGI_SWAP_CHAIN_DESC  desc;
		swapChain->GetDesc(&desc);
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		desc.OutputWindow = window;
		desc.Windowed = ((GetWindowLongPtr(window, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

		bufferCount = desc.BufferCount;
		D3D12_DESCRIPTOR_HEAP_DESC descriptorImGuiRender = {};
		descriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descriptorImGuiRender.NumDescriptors = bufferCount;
		descriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		if (device->CreateDescriptorHeap(&descriptorImGuiRender, IID_PPV_ARGS(&descriptorHeapImGuiRender)) != S_OK)
			return false;

		ID3D12CommandAllocator* allocator = nullptr;
		device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator));
		allocators = new ID3D12CommandAllocator * [bufferCount];
		for (UINT i = 0; i < bufferCount; i++) {
			allocators[i] = allocator;
		}

		if (device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocators[0], NULL, IID_PPV_ARGS(&commandList)) != S_OK
			|| commandList->Close() != S_OK)
			return false;

		contexts = new BackBufferContext[bufferCount];
		D3D12_DESCRIPTOR_HEAP_DESC descriptorBackBuffers;
		descriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		descriptorBackBuffers.NumDescriptors = bufferCount;
		descriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		descriptorBackBuffers.NodeMask = 1;
		if (device->CreateDescriptorHeap(&descriptorBackBuffers, IID_PPV_ARGS(&descriptorHeapBackBuffers)) != S_OK)
			return false;

		//createRTV(swapChain);

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX12_Init(device, bufferCount, DXGI_FORMAT_R8G8B8A8_UNORM,
			descriptorHeapImGuiRender,
			descriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
			descriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());
		ImGui_ImplDX12_CreateDeviceObjects();
		ImGui::GetIO().ImeWindowHandle = window;

		return true;
	}

	return false;
}

void DirectX12::createRTV(IDXGISwapChain3* swapChain) {
	const auto rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = descriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart();
	for (uint32_t i = 0; i < bufferCount; i++) {
		ID3D12Resource* pBackBuffer = nullptr;
		contexts[i].descriptorHandle = RTVHandle;
		swapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
		device->CreateRenderTargetView(pBackBuffer, nullptr, RTVHandle);
		contexts[i].resource = pBackBuffer;
		//ImFX::NewFrame(device, pBackBuffer, static_cast<float>(GetDpiForWindow(window)));

		RTVHandle.ptr += rtvDescriptorSize;
	}
}

void DirectX12::releaseRTV() {
	for (uint32_t i = 0; i < bufferCount; i++) {
		if (contexts[i].resource != nullptr)
			contexts[i].resource->Release();
	}
}

// DirectX 11 =========================================================

void DirectX11::onPresent(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {

	GuiData* guiData = GameData::getGuiData(); 
	if (guiData != nullptr) {
		if (!initialized) {
			if (SUCCEEDED(swapChain->GetDevice(IID_PPV_ARGS(&device)))) {
				device->GetImmediateContext(&context);
				DXGI_SWAP_CHAIN_DESC sd;
				swapChain->GetDesc(&sd);

				window = (HWND)FindWindowA(nullptr, (LPCSTR)"Minecraft");;

				ID3D11Texture2D* pBackBuffer;
				swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
				device->CreateRenderTargetView(pBackBuffer, NULL, &targetView);
				pBackBuffer->Release();

				DirectXHook::SetupImGui();
				ImGui_ImplWin32_Init(window);
				ImGui_ImplDX11_Init(device, context);

				initialized = true;
			}
			else {
				return;
			}
		}

		ImVec2 windowSize = ImVec2(guiData->widthReal, guiData->heightReal);
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame(windowSize);

		ImGui::NewFrame();
		DirectXHook::onRender2D();
		ImGui::EndFrame();

		ImGui::Render();
		context->OMSetRenderTargets(1, &targetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	return;
}

// https://github.com/rdbo/ImGui-DirectX-11-Kiero-Hook/issues/4
HRESULT DirectX11::onResizeBuffers(IDXGISwapChain* a1, UINT a2, UINT width, UINT height, DXGI_FORMAT a5, UINT a6) {
	if (targetView) {
		context->OMSetRenderTargets(0, 0, 0);
		targetView->Release();
	}

	HRESULT hr = oResizeBuffers(a1, a2, width, height, a5, a6);

	ID3D11Texture2D* pBuffer;
	a1->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);

	device->CreateRenderTargetView(pBuffer, NULL, &targetView);
	pBuffer->Release();

	context->OMSetRenderTargets(1, &targetView, NULL);

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context->RSSetViewports(1, &vp);

	return hr;
}

// Main =========================================================
HRESULT DirectXHook::onPresent(IDXGISwapChain3* swapChain, UINT syncInterval, UINT flags) {
	if (SUCCEEDED(swapChain->GetDevice(IID_PPV_ARGS(&DirectX11::device)))) {
		DirectX11::onPresent(swapChain, syncInterval, flags);
	}
	else if (SUCCEEDED(swapChain->GetDevice(IID_PPV_ARGS(&DirectX12::device)))) {
		//static_cast<ID3D12Device5*>(DirectX12::device)->RemoveDevice();
		DirectX12::onPresent(swapChain, syncInterval, flags);
	}

	return oPresent(swapChain, syncInterval, flags);
}

void DirectXHook::SetupImGui() {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	io.FontDefault = io.Fonts->AddFontFromMemoryTTF((void*)Mojangles, sizeof(Mojangles), 200.f);
	ImGuiStyle* style = &ImGui::GetStyle();
	style->AntiAliasedFill = false;
	style->AntiAliasedLines = false;
}

void DirectXHook::init() {
	// directx 12
	if (kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success) {
		kiero::bind(54, (void**)&DirectX12::oExecute, DirectX12::onExecuteCommandList);
		kiero::bind(140, (void**)&oPresent, onPresent);
		kiero::bind(84, (void**)&DirectX12::oDrawInstanced, DirectX12::onDrawInstanced);
		kiero::bind(85, (void**)&DirectX12::oDrawIndexedInstance, DirectX12::onDrawIndexedInstance);
	}
	// directx 11
	else if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
		kiero::bind(8, (void**)&oPresent, onPresent);
		kiero::bind(13, (void**)&DirectX11::oResizeBuffers, DirectX11::onResizeBuffers);
	}
}

void DirectXHook::shutdown() {
	kiero::shutdown();
}