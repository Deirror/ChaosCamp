#pragma once

#include <core.h>

struct RayTracingOutputContext {
    void Reset() {
        uavHeap.Reset();
        output.Reset();
    }

    ComPtr<ID3D12DescriptorHeap> uavHeap;   
    ComPtr<ID3D12Resource> output;

    // Cached GPU handles for fast binding during rendering.
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = {};
    D3D12_GPU_DESCRIPTOR_HANDLE uavGpuHandle = {};
};

struct RayTracingOutputBuilder {
    static RayTracingOutputContext Create(
        ID3D12Device*,
        UINT width,
        UINT height
    );

    static ComPtr<ID3D12Resource> CreateRTOutput(
        ID3D12Device*,
		ID3D12DescriptorHeap*,
        UINT width,
        UINT height
    );

    static ComPtr<ID3D12DescriptorHeap> CreateUAVHeap(
        ID3D12Device*
    );
};
