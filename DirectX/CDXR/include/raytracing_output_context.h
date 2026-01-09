#pragma once

#include <core.h>

struct RayTracingOutputContext {
    void Reset() {
        output.Reset();
        uavHeap.Reset();
    }

    ComPtr<ID3D12Resource> output;
    ComPtr<ID3D12DescriptorHeap> uavHeap;   
};

struct RayTracingOutputBuilder {
    static RayTracingOutputContext Create(
        ID3D12Device*,
        UINT width,
        UINT height
    );

    static ComPtr<ID3D12Resource> CreateRTOutput(
        ID3D12Device*,
        UINT width,
        UINT height
    );

    static ComPtr<ID3D12DescriptorHeap> CreateUAVHeap(
        ID3D12Device*
    );
};
