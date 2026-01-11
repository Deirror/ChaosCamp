#pragma once

#include <dxgi1_6.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxcapi.h>
//--------------
#include <wrl.h>
//---------------
#include <vector>
#include <array>
//--------------
#include "log.h"

using Microsoft::WRL::ComPtr;

struct Vertex {
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
};

template <typename Interface>
inline ComPtr<Interface> QueryAs(IUnknown* obj)
{
    if (!obj) {

        LOG_FATAL("Obj is nullptr.");
    }

    ComPtr<Interface> out;
    if (FAILED(obj->QueryInterface(IID_PPV_ARGS(&out)))) {

        LOG_FATAL("Unable to query interface.");
    }

    return out;
}

inline void Transition(
    ID3D12Resource* res, 
    D3D12_RESOURCE_STATES before, 
    D3D12_RESOURCE_STATES after,
	ID3D12GraphicsCommandList* cmdList
) {

	D3D12_RESOURCE_BARRIER barr;
	barr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barr.Transition.pResource = res;
	barr.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barr.Transition.StateBefore = before;
	barr.Transition.StateAfter = after;

	cmdList->ResourceBarrier(1, &barr);
}

inline void CopyResult(
	ID3D12Resource* dstRes, 
	ID3D12Resource* cpyRes,
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footPrint,
	ID3D12GraphicsCommandList* cmdList
) { 

	D3D12_TEXTURE_COPY_LOCATION texCopyLoc{};
	texCopyLoc.pResource = cpyRes;
	texCopyLoc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	texCopyLoc.SubresourceIndex = 0;

	D3D12_TEXTURE_COPY_LOCATION rbbCopyLoc{};
	rbbCopyLoc.pResource = dstRes;
	rbbCopyLoc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	rbbCopyLoc.PlacedFootprint = footPrint;

	cmdList->CopyTextureRegion(&rbbCopyLoc, 0, 0, 0, &texCopyLoc, nullptr);
}

inline ComPtr<ID3D12Resource> CreateBuffer(
	ID3D12Device* device,
	UINT64 size,
	D3D12_RESOURCE_FLAGS flags,
	D3D12_RESOURCE_STATES initState,
	D3D12_HEAP_TYPE heapType = D3D12_HEAP_TYPE_DEFAULT
) {

	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = heapType;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProps.CreationNodeMask = 1;
	heapProps.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC desc{};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = size;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = flags;

	ComPtr<ID3D12Resource> buffer;
	if (FAILED(device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		initState,
		nullptr,
		IID_PPV_ARGS(&buffer)
	))) {

		LOG_FATAL("Unable to create Committed Resource.");
	}

	return buffer;
}

inline void UploadDataToBuff(ID3D12Resource* buff, void* data, UINT size) {

    void* mapped = nullptr;
	if (FAILED(buff->Map(0, nullptr, &mapped))) {

		LOG_FATAL("Unable to map Buff.");
	}

    memcpy(mapped, data, size);

    buff->Unmap(0, nullptr);
}

inline std::array<float, 12> DefaultTransform() {
	std::array<float, 12> m{};
	m[0] = 1.0f; // row 0
	m[5] = 1.0f; // row 1
	m[10] = 1.0f; // row 2
	return m;
}

inline void FillIdentityTransform3x4(float out[3][4]) {
	// row-major 3x4 identity
	memset(out, 0, sizeof(float) * 3 * 4);
	out[0][0] = 1.0f;
	out[1][1] = 1.0f;
	out[2][2] = 1.0f;
}

inline UINT64 AlignUp(UINT64 v, UINT64 a) {

	return (v + (a - 1)) & ~(a - 1); 
}

inline void CreateTLASShaderResourceView(
	ID3D12Device* device,
	ID3D12Resource* tlasResult,
	ID3D12DescriptorHeap* heap,
	UINT slotIndex = 0
) {

	UINT inc = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (SIZE_T)inc * slotIndex;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.RaytracingAccelerationStructure.Location = tlasResult->GetGPUVirtualAddress();

	device->CreateShaderResourceView(nullptr, &srvDesc, handle);
}

