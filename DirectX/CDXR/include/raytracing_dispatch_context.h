#pragma once

#include <core.h>
#include <frame_context.h>

struct RayTracingDispatchContext {
    void Reset() {
        sbtUpload.Reset();
        sbtDefault.Reset();
        desc = {};
    }

    void UpdateDimensions(UINT width, UINT height) {
        desc.Width = width;
        desc.Height = height;
    }

    ComPtr<ID3D12Resource> sbtUpload;
    ComPtr<ID3D12Resource> sbtDefault;
    D3D12_DISPATCH_RAYS_DESC desc{};
};

struct RayTracingDispatchBuilder {
    static RayTracingDispatchContext Create(
		FrameContext&, 
		ID3D12StateObjectProperties*,
        ID3D12Device*,
		UINT width,
		UINT height
    );

    static ComPtr<ID3D12Resource> CreateSBTBuff(
        ID3D12Device*,
        D3D12_HEAP_TYPE,
		D3D12_RESOURCE_STATES, 
        UINT size
    );

    static D3D12_DISPATCH_RAYS_DESC PrepareDispatchRayDesc(
		ID3D12Resource*,
        UINT recordSize,
		UINT rayGenOffs, 
		UINT missOffs, 
		UINT hitGroupOffs, 
		UINT width,
		UINT height
    );
};
