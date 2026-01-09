#pragma once

#include <core.h>

/// Consists of the rendering synchorization part.
struct FenceContext {
    void Signal(ID3D12CommandQueue* cmdQueue, UINT64& outFrameVal);
    void Wait(UINT64 val);

    ComPtr<ID3D12Fence> fence;
    HANDLE event = nullptr;
    UINT64 nextVal = 1;
};

struct FenceBuilder {
    static FenceContext Create(ID3D12Device*);

    static ComPtr<ID3D12Fence> CreateFence(ID3D12Device*);
    static HANDLE CreateFenceEvent();
};
