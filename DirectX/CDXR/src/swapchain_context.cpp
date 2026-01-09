#include "swapchain_context.h"

void SwapChainContext::ReallocRTVs(ID3D12Device* device, UINT bufferCount) {

	rtvHandles = SwapChainBuilder::CreateRTVHandles(
		swapChain.Get(),
		device, 
		rtvHeap.Get(),
		backBuffs,
		bufferCount
	);
}

void SwapChainContext::Reset() {

	for (int idx = 0; idx < buffCount; ++idx) {

		backBuffs[idx].Reset();
	}
}

void SwapChainContext::ChangeCurrIdx() {

	// Supports get func for curr Back Buff idx.
	ComPtr<IDXGISwapChain3> swapChain3 = QueryAs<IDXGISwapChain3>(swapChain.Get());
	currIdx = swapChain3->GetCurrentBackBufferIndex();
}

ID3D12Resource* SwapChainContext::CurrBackBuff() {

	return backBuffs[currIdx].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE SwapChainContext::CurrRTV() {

	return rtvHandles[currIdx];
}

SwapChainContext SwapChainBuilder::Create(
	IDXGIFactory* factory,
	ID3D12CommandQueue* cmdQueue,
	ID3D12Device* device,
	HWND hwnd,
	UINT width,
	UINT height,
	UINT bufferCount
) {

	SwapChainContext swpCh;

	swpCh.buffCount = bufferCount;

	swpCh.swapChain = SwapChainBuilder::CreateSwapChain(
		factory, 
		cmdQueue, 
		hwnd, 
		width, 
		height, 
		bufferCount
	);

	swpCh.rtvHeap = SwapChainBuilder::CreateRTVHeap(device, bufferCount);
	swpCh.rtvHandles = SwapChainBuilder::CreateRTVHandles(
		swpCh.swapChain.Get(),
		device, 
		swpCh.rtvHeap.Get(),
		swpCh.backBuffs,
		bufferCount
	);

	return swpCh;
}

ComPtr<ID3D12DescriptorHeap> SwapChainBuilder::CreateRTVHeap(
	ID3D12Device* device,
	UINT bufferCount
) {

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descHeapDesc.NumDescriptors = bufferCount;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    ComPtr<ID3D12DescriptorHeap> rtvHeap;
	if (FAILED(device->CreateDescriptorHeap(
		&descHeapDesc,
		IID_PPV_ARGS(&rtvHeap)))) {

		LOG_FATAL("Unable to create Descriptor Heap for RTVs.");
	}

	return rtvHeap;
}

std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> SwapChainBuilder::CreateRTVHandles(
	IDXGISwapChain* swapChain, 
	ID3D12Device* device, 
	ID3D12DescriptorHeap* rtvHeap, 
	std::vector<ComPtr<ID3D12Resource>>& backBuffs, 
	UINT bufferCount
) {

    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHandles(bufferCount);

	backBuffs.resize(bufferCount);

	for (size_t idx = 0; idx < bufferCount; ++idx) {

		if (FAILED(swapChain->GetBuffer(idx, IID_PPV_ARGS(&backBuffs[idx])))) {

			LOG_FATAL("Unable to get buffers for RTVs.");
		}
		
		rtvHandles[idx] = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandles[idx].ptr += idx * device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		D3D12_RENDER_TARGET_VIEW_DESC rtv{};
		rtv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtv.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtv.Texture2D.MipSlice = 0;
		rtv.Texture2D.PlaneSlice = 0;

		device->CreateRenderTargetView(backBuffs[idx].Get(), &rtv, rtvHandles[idx]);
	}	

	return rtvHandles;
}

ComPtr<IDXGISwapChain> SwapChainBuilder::CreateSwapChain(
	IDXGIFactory* factory,
	ID3D12CommandQueue* cmdQueue,
	HWND hwnd,
	UINT width,
	UINT height,
	UINT bufferCount
) {

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = bufferCount;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

	// Supports swpch func for hwnd.
	ComPtr<IDXGIFactory2> factory2 = QueryAs<IDXGIFactory2>(factory);

    ComPtr<IDXGISwapChain1> swapChain1;
	if (FAILED(factory2->CreateSwapChainForHwnd(
		cmdQueue, hwnd, &swapChainDesc,
		nullptr, nullptr, &swapChain1))) {

		LOG_FATAL("Unable to create SwapChain.");
	}

	return QueryAs<IDXGISwapChain>(swapChain1.Get());
}
