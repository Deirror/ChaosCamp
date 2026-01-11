#include "cdxr_renderer.h"

#include <fstream>
//-----------------
#include "d3dx12.h"
#include "shapes.hpp"

namespace cdxr {

void CDXRenderer::testClearAndSetRT() {

	float clearColor[] = { .4f, .1f, .8f, 1.f };

	D3D12_CPU_DESCRIPTOR_HANDLE rtv = swpCh.CurrRTV();

	frame.cmdList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
	frame.cmdList->OMSetRenderTargets(1, &rtv, false, nullptr);
}

void CDXRenderer::beginFrame() {

	frame.Reset(); 

	Transition(
		swpCh.CurrBackBuff(),
		D3D12_RESOURCE_STATE_PRESENT, 
		D3D12_RESOURCE_STATE_RENDER_TARGET, 
		frame.cmdList.Get()
	);

	D3D12_CPU_DESCRIPTOR_HANDLE rtv = swpCh.CurrRTV();

	frame.cmdList->OMSetRenderTargets(1, &rtv, false, nullptr);
	frame.cmdList->ClearRenderTargetView(rtv, clearColor.data(), 0, nullptr);
}

void CDXRenderer::writeImageToFile() {

	void* data;
	if (FAILED(rbb.buff->Map(0, nullptr, &data))) {

		LOG_FATAL("Unable to map data from RBB.");
	}

	std::string name("result.ppm");
	std::ofstream ofs(name, std::ios::binary);

	if (!ofs.is_open()) {

		LOG_FATAL("Unable to open file for writing.");
	}

	D3D12_RESOURCE_DESC desc = swpCh.CurrBackBuff()->GetDesc();
	ofs << "P3\n" << desc.Width << " " << desc.Height << "\n255\n";

	for (uint32_t rowIdx = 0; rowIdx < desc.Height; ++rowIdx) {

		uint32_t rowPitch = rbb.footPrint.Footprint.RowPitch;
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
	rbb.buff->Unmap(0, nullptr);
}

void CDXRenderer::endFrame() {

	if (mode == RenderMode::RayTraced) {

		Transition(
			rtOut.output.Get(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, 
			D3D12_RESOURCE_STATE_COPY_SOURCE, 
			frame.cmdList.Get()
		);

		Transition(
			swpCh.CurrBackBuff(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, 
			D3D12_RESOURCE_STATE_COPY_DEST, 
			frame.cmdList.Get()
		);

		frame.cmdList->CopyResource(
			swpCh.CurrBackBuff(), 
			rtOut.output.Get()
		);

		Transition(
			rtOut.output.Get(),
			D3D12_RESOURCE_STATE_COPY_SOURCE, 
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, 
			frame.cmdList.Get()
		);

		Transition(
			swpCh.CurrBackBuff(),
			D3D12_RESOURCE_STATE_COPY_DEST, 
			D3D12_RESOURCE_STATE_RENDER_TARGET, 
			frame.cmdList.Get()
		);
	}

	Transition(
		swpCh.CurrBackBuff(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, 
		D3D12_RESOURCE_STATE_PRESENT, 
		frame.cmdList.Get()
	);

	frame.Close();   

	execute();         
	flushGPU();       

	swpCh.ChangeCurrIdx();
}

void CDXRenderer::execute(bool present) {

	ID3D12CommandList* cmdLists[] = { frame.cmdList.Get() };
	d3d.cmdQueue->ExecuteCommandLists(1, cmdLists);

	if (!present) {
		return;
	}

	swpCh.swapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
}

void CDXRenderer::flushGPU() {

	UINT64 waitForVal = 0;
	fence.Signal(d3d.cmdQueue.Get(), waitForVal);

	fence.Wait(waitForVal);
}

void CDXRenderer::init(HWND handle) {

// === DirectX Facotry, Device And Command Queue Creation === //
	d3d = D3DBuilder::Create();

	// Really helpful for doing debug breaks. 
	ComPtr<ID3D12InfoQueue> infoQ = QueryAs<ID3D12InfoQueue>(d3d.device.Get());
	infoQ->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
	infoQ->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);

// === Command Managers Creation === //
	frame = FrameBuilder::Create(d3d.device.Get());
	fence = FenceBuilder::Create(d3d.device.Get());

// === Front-Back Buffers Creation === //
	if (!handle) {
		return;
	}

	swpCh = SwapChainBuilder::Create(
		d3d.factory.Get(),
		d3d.cmdQueue.Get(),
		d3d.device.Get(),
		handle, 
		width, 
		height, 
		frameCount
	);

// === ReadBack Buffer Creation === //
	rbb = ReadbackBuilder::Create(
		d3d.device.Get(),
		swpCh.CurrBackBuff()
	);

// === Vertex Buffer Data Prep And Creation === //
	BuildSpiralRibbonTriangles(vertexData);
	Build_DEIRRORz_X_Triangles(vertexData);

	frame.Reset();

	vb = VertexBufferBuilder::Create(
		d3d.device.Get(), 
		frame.cmdList.Get(), 
		vertexData.data(), 
		vertexCount, 
		sizeof(Vertex)
	);

	frame.Close();

	execute(false);
	flushGPU();

// === Raster Pipeline Creation - Signatures, Rects, Scissors === //
	rasterPipe = RasterPipelineBuilder::Create(
		d3d.device.Get(),
		width, 
		height
	);

// === Raytracing Contexts Creation === //
	rtOut = RayTracingOutputBuilder::Create(
		d3d.device.Get(), 
		width, 
		height
	);

	rtPipe = RayTracingPipelineBuilder::Create(
		d3d.device.Get()
	);

	frame.Reset();

	accSt = AccelerationStructureBuilder::Create(
		d3d.device.Get(),
		frame.cmdList.Get(),
		vb.defaultBuff.Get(),
		vertexCount,
		sizeof(Vertex)
	);

	frame.Close();

	execute(false);
	flushGPU();

	CreateTLASShaderResourceView(
		d3d.device.Get(), 
		accSt.tlas.result.Get(), 
		rtOut.uavHeap.Get()
	);

	ComPtr<ID3D12StateObjectProperties> props =
		QueryAs<ID3D12StateObjectProperties>(rtPipe.stateObj.Get());

	rtDisp = RayTracingDispatchBuilder::Create(
		frame,
		props.Get(), 
		d3d.device.Get(),
		width,
		height
	);

	execute(false);
	flushGPU();
}

void CDXRenderer::render(const FrameData& data) {

	beginFrame();
	
	if (mode == RenderMode::Rasterized) {

		frame.cmdList->SetPipelineState(rasterPipe.pipelineState.Get());
		frame.cmdList->SetGraphicsRootSignature(rasterPipe.rootSignature.Get());

		frame.cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		frame.cmdList->IASetVertexBuffers(0, 1, &vb.view);

		frame.cmdList->RSSetViewports(1, &rasterPipe.viewport);
		frame.cmdList->RSSetScissorRects(1, &rasterPipe.scissor);

		frame.cmdList->SetGraphicsRoot32BitConstant(0, data.frameIdx, 0);
		frame.cmdList->SetGraphicsRoot32BitConstant(0, data.colorIdx, 1);
		frame.cmdList->SetGraphicsRoot32BitConstant(0, *reinterpret_cast<const UINT*>(&data.offsX), 2);
		frame.cmdList->SetGraphicsRoot32BitConstant(0, *reinterpret_cast<const UINT*>(&data.offsY), 3);

		frame.cmdList->DrawInstanced(vertexCount, (vertexCount / 3), 0, 0);
	}
	else {

		ID3D12DescriptorHeap* heaps[] = { rtOut.uavHeap.Get() };
		frame.cmdList->SetDescriptorHeaps(_countof(heaps), heaps);

		frame.cmdList->SetComputeRootSignature(rtPipe.globalRootSig.Get());

		frame.cmdList->SetComputeRootDescriptorTable(0, rtOut.srvGpuHandle);
		frame.cmdList->SetComputeRootDescriptorTable(1, rtOut.uavGpuHandle);

		// Supports for ray-tracing commands.
		ComPtr<ID3D12GraphicsCommandList4> cmdList4 = QueryAs<ID3D12GraphicsCommandList4>(frame.cmdList.Get());
		cmdList4->SetPipelineState1(rtPipe.stateObj.Get());

		cmdList4->DispatchRays(&rtDisp.desc);
	}

	endFrame();
}

void CDXRenderer::resize(int width, int height) {

	if (this->width == width &&
		this->height == height) {
		return; 
	}

	flushGPU();

	rtOut.Reset();
	swpCh.Reset();    

	if (FAILED(swpCh.swapChain->ResizeBuffers(
		frameCount, width, height,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH |
		DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING))) {

		LOG_FATAL("Unable to resize Buffers.");
	}

	swpCh.ReallocRTVs(d3d.device.Get(), frameCount); 

	this->width = width;
	this->height = height;

	swpCh.ChangeCurrIdx();

	rasterPipe.Resize(width, height);

	rtOut = RayTracingOutputBuilder::Create(d3d.device.Get(), width, height);
	CreateTLASShaderResourceView(d3d.device.Get(), accSt.tlas.result.Get(), rtOut.uavHeap.Get(), 0);

	rtDisp.desc.Width = width;
	rtDisp.desc.Height = height;
}

void CDXRenderer::setClearColor(const std::array<float, 4>& color) {

	clearColor = color;
}

void CDXRenderer::updateRenderMode() {

	stopRendering();

	mode = RenderMode((mode + 1) % 2);
}

std::vector<GPUVendorInfo> CDXRenderer::getVendorsInfo() {

	std::vector<GPUVendorInfo > vendorInfo;

	UINT idx = 0;
	ComPtr<IDXGIAdapter> adapter;
	while (true) {

		if (FAILED(d3d.factory->EnumAdapters(idx++, &adapter))) {
			break;
		}

		DXGI_ADAPTER_DESC desc;
		if (FAILED(adapter->GetDesc(&desc))) {
			continue;
		}

		vendorInfo.push_back(GPUVendorInfo(desc.Description, desc.VendorId, desc.DedicatedVideoMemory));
	}

	return vendorInfo;
}

}
