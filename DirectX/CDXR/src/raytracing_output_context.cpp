#include "raytracing_output_context.h"

RayTracingOutputContext RayTracingOutputBuilder::Create(
	ID3D12Device* device,
	UINT width,
	UINT height
) {

	RayTracingOutputContext rtOutCtx;

	rtOutCtx.uavHeap = CreateUAVHeap(device);
	rtOutCtx.output = CreateRTOutput(device, rtOutCtx.uavHeap.Get(), width, height);

	UINT inc = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	rtOutCtx.srvGpuHandle = rtOutCtx.uavHeap->GetGPUDescriptorHandleForHeapStart(); // TLAS
	rtOutCtx.uavGpuHandle = rtOutCtx.srvGpuHandle; // Output
	rtOutCtx.uavGpuHandle.ptr += inc; 

	return rtOutCtx;
}

ComPtr<ID3D12Resource> RayTracingOutputBuilder::CreateRTOutput(
	ID3D12Device* device, 
	ID3D12DescriptorHeap* uavHeap,
	UINT width, 
	UINT height
) {

	D3D12_RESOURCE_DESC texDesc{};
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Alignment = 0;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

	ComPtr<ID3D12Resource> rtOutput;
	if (FAILED(device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &texDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr, IID_PPV_ARGS(&rtOutput)
	))) {

		LOG_FATAL("Unable to create RayTracing Output.");
	}

	UINT inc = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = uavHeap->GetCPUDescriptorHandleForHeapStart();
	cpuHandle.ptr += inc * 1; 

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;

	device->CreateUnorderedAccessView(
		rtOutput.Get(),
		nullptr, &uavDesc,
		cpuHandle	
	);

	return rtOutput;
}

ComPtr<ID3D12DescriptorHeap> RayTracingOutputBuilder::CreateUAVHeap(
	ID3D12Device* device
) {

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.NumDescriptors = 2;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	ComPtr<ID3D12DescriptorHeap> uavHeap;
	if (FAILED(device->CreateDescriptorHeap(
		&heapDesc, IID_PPV_ARGS(&uavHeap)
	))) {

		LOG_FATAL("Unable to create UAV Heap.");
	}

	return uavHeap;
}
