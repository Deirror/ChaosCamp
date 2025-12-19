#include "cdxr_renderer.h"

#include <fstream>
//--------------------------
#include "TriangleVS.hlsl.h" // To include the file, hit compile on the .hlsl file in the dir.
#include "Triangle.hlsl.h" // To include the file, hit compile on the .hlsl file in the dir.
//-----------------
#include "d3dx12.h"

namespace cdxr {

void CDXRenderer::createFactoryAndAdapters() {

	if (FAILED(CreateDXGIFactory2(0,
		IID_PPV_ARGS(&factory)))) {

		LOG_FATAL("Unable to create DXGI Factory.");
	}

	if (FAILED(factory->EnumAdapterByGpuPreference(0,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(&adapter)))) {

		LOG_FATAL("Unable to create Adapter.");
	}
}

void CDXRenderer::createDevice() {

	if (FAILED(D3D12CreateDevice(
		adapter.Get(),
		D3D_FEATURE_LEVEL_12_0,
		IID_PPV_ARGS(&device)))) {

		LOG_FATAL("Unable to create Device.");
	}
}

void CDXRenderer::createCmdInterfaces() {

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	if (FAILED(device->CreateCommandQueue(
		&cmdQueueDesc,
		IID_PPV_ARGS(&cmdQueue)))) {

		LOG_FATAL("Unable to create Cmd Queue.");
	}

	if (FAILED(device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAlloc)))) {

		LOG_FATAL("Unable to create Cmd Allocator.");
	}

	if (FAILED(device->CreateCommandList1(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		D3D12_COMMAND_LIST_FLAG_NONE,
		IID_PPV_ARGS(&cmdList)))) {

		LOG_FATAL("Unable to create Cmd Allocator.");
	}

	cmdList->Close();

}

void CDXRenderer::createFence() {

	if (FAILED(device->CreateFence(0, 
		D3D12_FENCE_FLAG_NONE, 
		IID_PPV_ARGS(&fence)))) {

		LOG_FATAL("Unable to create Fence.");
	}

	fenceIdx = 0;
	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	if (!fenceEvent) {

		LOG_FATAL("Unable to create Event.");
	}
}

void CDXRenderer::testClearAndSetRT() {

	float clearColor[] = { .4f, .1f, .8f, 1.f };
	cmdList->ClearRenderTargetView(rtvHandles[currBuffIdx], clearColor, 0, nullptr);

	cmdList->OMSetRenderTargets(1, &rtvHandles[currBuffIdx], false, nullptr);
}

void CDXRenderer::createSwapChain(HWND handle) {

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = frameCount;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

	ComPtr<IDXGISwapChain1> tmpSwapChain;
	if (FAILED(factory->CreateSwapChainForHwnd(
		cmdQueue.Get(), handle, &swapChainDesc,
		nullptr, nullptr, &tmpSwapChain))) {

		LOG_FATAL("Unable to create SwapChain.");
	}

	if (FAILED(tmpSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain)))) {

		LOG_FATAL("Unable to query interface for SwapChain3.");
	}
}

void CDXRenderer::createRTVHeap() {

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descHeapDesc.NumDescriptors = frameCount;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	if (FAILED(device->CreateDescriptorHeap(
		&descHeapDesc,
		IID_PPV_ARGS(&rtvHeap)))) {

		LOG_FATAL("Unable to create Descriptor Heap for RTVs.");
	}
}

void CDXRenderer::createRTVHandles() {

	for (size_t idx = 0; idx < frameCount; ++idx) {

		if (FAILED(swapChain->GetBuffer(idx, IID_PPV_ARGS(&buffers[idx])))) {

			LOG_FATAL("Unable to get buffers for RTVs.");
		}
		
		rtvHandles[idx] = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandles[idx].ptr += idx * device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		D3D12_RENDER_TARGET_VIEW_DESC rtv{};
		rtv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtv.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtv.Texture2D.MipSlice = 0;
		rtv.Texture2D.PlaneSlice = 0;

		device->CreateRenderTargetView(buffers[idx].Get(), &rtv, rtvHandles[idx]);
	}	
}

void CDXRenderer::createRBBHandle() {

	D3D12_RESOURCE_DESC1 desc = buffers[currBuffIdx]->GetDesc1();

	uint64_t rbbSize = 0;
	device->GetCopyableFootprints1(
		&desc, 
		0, 1, 0,
		&rtFootprint,
		nullptr,
		nullptr,
		&rbbSize
		);

	D3D12_RESOURCE_DESC readBackBuff{};
	readBackBuff.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	readBackBuff.Width = rbbSize;
	readBackBuff.Height = 1;
	readBackBuff.MipLevels = 1;
	readBackBuff.SampleDesc.Count = 1;
	readBackBuff.DepthOrArraySize = 1;
	readBackBuff.Format = DXGI_FORMAT_UNKNOWN;
	readBackBuff.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_HEAP_PROPERTIES rbbHeapProps{};
	rbbHeapProps.Type = D3D12_HEAP_TYPE_READBACK;

	if (FAILED(device->CreateCommittedResource(
		&rbbHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&readBackBuff,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&rbbHandle)))) {

		LOG_FATAL("Unable to create RBB Resource.");
	}
}

void CDXRenderer::createVertexBuffer() {
	
	Vertex triVertices[] = {
		{ 0.f, 0.5f },
		{ 0.5f, -0.5f },
		{ -0.5f, -0.5f }
	};

	D3D12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(triVertices));

	if (FAILED(device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuff)))) {

		LOG_FATAL("Unable to create Vertex Buffer.");
	}

	void* pVertexData = nullptr;
	if (FAILED(vertexBuff->Map(0 , nullptr, &pVertexData))) {

		LOG_FATAL("Unable to map Vertex Buffer.");
	}

	memcpy(pVertexData, triVertices, sizeof(triVertices));

	vertexBuff->Unmap(0, nullptr);

	vertexBuffView = D3D12_VERTEX_BUFFER_VIEW{};
	vertexBuffView.BufferLocation = vertexBuff->GetGPUVirtualAddress();
	vertexBuffView.StrideInBytes = sizeof(Vertex);   
	vertexBuffView.SizeInBytes = sizeof(triVertices); 
}

void CDXRenderer::createRootSignature() {

	D3D12_ROOT_SIGNATURE_DESC rootSignDesc{};
	rootSignDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;

	if (FAILED(D3D12SerializeRootSignature(
		&rootSignDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&signature,
		&error))) {

		LOG_FATAL("Unable to serialize Root Signature.");
	}

	if (FAILED(device->CreateRootSignature(0,
		signature->GetBufferPointer(),
		signature->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)))) {

		LOG_FATAL("Unable to create Root Signature.");
	}
}

void CDXRenderer::createPipelineState() {

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	psoDesc.pRootSignature = rootSignature.Get();
	psoDesc.PS = { g_triangle, _countof(g_triangle) };
	psoDesc.VS = { g_trianglevs, _countof(g_trianglevs) };
	psoDesc.InputLayout = { inputLayout, _countof(inputLayout)};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = false;
	psoDesc.DepthStencilState.StencilEnable = false;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;
	
	if (FAILED(device->CreateGraphicsPipelineState(
		&psoDesc, IID_PPV_ARGS(&pipelineState)))) {

		LOG_FATAL("Unable to create Graphics Pipeline.");
	}
}

void CDXRenderer::createViewport() {

	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	scissorRect.top = 0;
	scissorRect.left = 0;
	scissorRect.right = width;
	scissorRect.bottom = height;
}

void CDXRenderer::beginFrame() {

	if (FAILED(cmdAlloc->Reset())) {

		LOG_FATAL("Unable to reset Cmd Alloc.");
	}

	if (FAILED(cmdList->Reset(cmdAlloc.Get(), nullptr))) {

		LOG_FATAL("Unable to reset Cmd List.");
	}

	transition(buffers[currBuffIdx].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	cmdList->OMSetRenderTargets(1, &rtvHandles[currBuffIdx], false, nullptr);
	cmdList->ClearRenderTargetView(rtvHandles[currBuffIdx], clearColor.data(), 0, nullptr);
}

void CDXRenderer::copyResult(ID3D12Resource* dstRes, ID3D12Resource* cpyRes) {

	D3D12_TEXTURE_COPY_LOCATION texCopyLoc{};
	texCopyLoc.pResource = cpyRes;
	texCopyLoc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	texCopyLoc.SubresourceIndex = 0;

	D3D12_TEXTURE_COPY_LOCATION rbbCopyLoc{};
	rbbCopyLoc.pResource = dstRes;
	rbbCopyLoc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	rbbCopyLoc.PlacedFootprint = rtFootprint;

	cmdList->CopyTextureRegion(&rbbCopyLoc, 0, 0, 0, &texCopyLoc, nullptr);
}

void CDXRenderer::writeImageToFile() {

	void* data;
	if (FAILED(rbbHandle->Map(0, nullptr, &data))) {

		LOG_FATAL("Unable to map data from RBB.");
	}

	std::string name("result.ppm");
	std::ofstream ofs(name, std::ios::binary);

	if (!ofs.is_open()) {

		LOG_FATAL("Unable to open file for writing.");
	}

	D3D12_RESOURCE_DESC1 desc = buffers[currBuffIdx]->GetDesc1();
	ofs << "P3\n" << desc.Width << " " << desc.Height << "\n255\n";

	for (uint32_t rowIdx = 0; rowIdx < desc.Height; ++rowIdx) {

		uint32_t rowPitch = rtFootprint.Footprint.RowPitch;
		uint8_t* rowData = reinterpret_cast<uint8_t*>(data) + rowIdx * rowPitch;

		for (uint32_t colIdx = 0; colIdx < desc.Width; ++colIdx) {

			uint8_t* pixelData = rowData + colIdx * RGBA_COLOR_CHANNELS_COUNT;
			for (int channelIdx = 0; channelIdx < RGBA_COLOR_CHANNELS_COUNT - 1; ++channelIdx) {

				ofs << static_cast<int>(pixelData[channelIdx]) << " ";
			}
		}

		ofs << "\n";
	}

	ofs.close();
	rbbHandle->Unmap(0, nullptr);
}

void CDXRenderer::endFrame() {
		
	transition(buffers[currBuffIdx].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	close();
	execute();

	waitForGPU();

	currBuffIdx = swapChain->GetCurrentBackBufferIndex();
}

void CDXRenderer::transition(ID3D12Resource* res, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) {

	D3D12_RESOURCE_BARRIER barr;
	barr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barr.Transition.pResource = res;
	barr.Transition.Subresource = 0;
	barr.Transition.StateBefore = before;
	barr.Transition.StateAfter = after;

	cmdList->ResourceBarrier(1, &barr);
}

void CDXRenderer::close() {

	if (FAILED(cmdList->Close())) {

		LOG_FATAL("Unable to close Cmd List.");
	}
}

void CDXRenderer::execute() {

	ID3D12CommandList* cmdLists[] = { cmdList.Get() };
	cmdQueue->ExecuteCommandLists(1, cmdLists);

	swapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
}

void CDXRenderer::waitForGPU() {

	UINT64 waitForVal = ++fenceIdx;

	cmdQueue->Signal(fence.Get(), waitForVal);

	if (fence->GetCompletedValue() >= waitForVal) {
		return;
	}

	if (FAILED(fence->SetEventOnCompletion(waitForVal, fenceEvent))) {

		LOG_FATAL("Unable to set Event on completion.");
	}

	WaitForSingleObject(fenceEvent, INFINITE);
}

void CDXRenderer::init(HWND handle) {

	createFactoryAndAdapters();
	createDevice();

	createCmdInterfaces();

	createFence();

	if (!handle) {
		return;
	}

	createSwapChain(handle);

	createRTVHeap();
	createRTVHandles();

	createRBBHandle();

	createVertexBuffer();
	createViewport();

	createRootSignature();
	createPipelineState();
}

void CDXRenderer::render() {

	beginFrame();

	cmdList->SetPipelineState(pipelineState.Get());
	cmdList->SetGraphicsRootSignature(rootSignature.Get());

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->IASetVertexBuffers(0, 1, &vertexBuffView);

	cmdList->RSSetViewports(1, &viewport);
	cmdList->RSSetScissorRects(1, &scissorRect);

	cmdList->DrawInstanced(3, 1, 0, 0);

	endFrame();
}

void CDXRenderer::resize(int width, int height) {

	if (this->width == width &&
		this->height == height) {
		return; 
	}

	waitForGPU();

	for (int idx = 0; idx < frameCount; ++idx) {

		buffers[idx].Reset();
	}

	if (FAILED(swapChain->ResizeBuffers(
		frameCount, width, height,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH |
		DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING))) {

		LOG_FATAL("Unable to resize buffers.");
	}

	createRTVHandles();

	this->width = width;
	this->height = height;

	currBuffIdx = swapChain->GetCurrentBackBufferIndex();

	createViewport();
}

void CDXRenderer::setClearColor(const std::array<float, 4>& color) {

	clearColor = color;
}

std::vector<GPUVendorInfo> CDXRenderer::getVendorsInfo() {

	std::vector<GPUVendorInfo > vendorInfo;

	UINT idx = 0;
	ComPtr<IDXGIAdapter1> adapter;
	while (true) {
		if (FAILED(factory->EnumAdapters1(idx++, &adapter))) {
			break;
		}

		DXGI_ADAPTER_DESC1 desc;
		if (FAILED(adapter->GetDesc1(&desc))) {
			continue;
		}

		vendorInfo.push_back(GPUVendorInfo(desc.Description, desc.VendorId, desc.DedicatedVideoMemory));
	}

	return vendorInfo;
}

}
