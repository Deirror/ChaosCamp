#include "raytracing_output_context.h"

RayTracingOutputContext RayTracingOutputBuilder::Create(
	ID3D12Device* device,
	UINT width,
	UINT height
) {

	RayTracingOutputContext rtOutCtx;

	rtOutCtx.output = CreateRTOutput(device, width, height);
	rtOutCtx.uavHeap = CreateUAVHeap(device);

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;

	device->CreateUnorderedAccessView(
		rtOutCtx.output.Get(),
		nullptr, &uavDesc,
		rtOutCtx.uavHeap->GetCPUDescriptorHandleForHeapStart()
	);

	return rtOutCtx;
}

ComPtr<ID3D12Resource> RayTracingOutputBuilder::CreateRTOutput(
	ID3D12Device* device, 
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
		nullptr, IID_PPV_ARGS(&rtOutput)))) {

		LOG_FATAL("Unable to create RayTracing Output.");
	}

	return rtOutput;
}

ComPtr<ID3D12DescriptorHeap> RayTracingOutputBuilder::CreateUAVHeap(
	ID3D12Device* device
) {

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	ComPtr<ID3D12DescriptorHeap> uavHeap;
	if (FAILED(device->CreateDescriptorHeap(
		&heapDesc, IID_PPV_ARGS(&uavHeap)))) {

		LOG_FATAL("Unable to create UAV Heap.");
	}

	return uavHeap;
}
