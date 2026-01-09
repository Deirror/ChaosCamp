#pragma once

#include <core.h>

/// Represents per-frame data such as registered commands.
struct FrameContext {
    void Reset();
    void Close();

    ComPtr<ID3D12CommandAllocator> cmdAlloc;
    ComPtr<ID3D12GraphicsCommandList> cmdList;

    UINT64 fenceVal = 0;
};

struct FrameBuilder {
    static FrameContext Create(ID3D12Device*);

    static ComPtr<ID3D12CommandAllocator> CreateCmdAlloc(ID3D12Device*);

    static ComPtr<ID3D12GraphicsCommandList> CreateCmdList(
        ID3D12Device*, ID3D12CommandAllocator* 
    );
};
