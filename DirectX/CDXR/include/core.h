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
#include <log.h>

using Microsoft::WRL::ComPtr;

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
	barr.Transition.Subresource = 0;
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

struct Vertex {
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
};
