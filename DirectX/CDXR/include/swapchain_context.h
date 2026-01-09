#pragma once

#include <core.h>

/// Manages the textures(buffers) which are used for writing and reading(displaying on the screen).
/// Two buffers - front and back - are more than enough.
struct SwapChainContext {
    void ReallocRTVs(ID3D12Device* device, UINT bufferCount);

    void Reset(); 

    void ChangeCurrIdx(); 

    ID3D12Resource* CurrBackBuff(); 
    D3D12_CPU_DESCRIPTOR_HANDLE CurrRTV();

    ComPtr<IDXGISwapChain> swapChain;

    std::vector<ComPtr<ID3D12Resource>> backBuffs;
    ComPtr<ID3D12DescriptorHeap> rtvHeap;
    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHandles;

    UINT buffCount = 0;
    UINT currIdx = 0;
};

struct SwapChainBuilder {
    static SwapChainContext Create(
        IDXGIFactory*,
        ID3D12CommandQueue*,
        ID3D12Device*,
        HWND hwnd,
        UINT width,
        UINT height,
        UINT bufferCount
    );

    static ComPtr<IDXGISwapChain> CreateSwapChain(
        IDXGIFactory*,
        ID3D12CommandQueue*,
        HWND hwnd,
        UINT width,
        UINT height,
        UINT bufferCount
    );

    static ComPtr<ID3D12DescriptorHeap> CreateRTVHeap(
        ID3D12Device*,
        UINT bufferCount
    );

    static std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> CreateRTVHandles(
        IDXGISwapChain*,
        ID3D12Device*,
		ID3D12DescriptorHeap* rtvHeap,
		std::vector<ComPtr<ID3D12Resource>>& backBuffs,
        UINT bufferCount
    );
};
