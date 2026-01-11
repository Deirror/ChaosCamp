#pragma once

#include "core.h"

struct D3DContext {
    ComPtr<IDXGIFactory> factory;
    ComPtr<IDXGIAdapter> adapter;
    ComPtr<ID3D12Device> device;
    ComPtr<ID3D12CommandQueue> cmdQueue;
};

struct D3DBuilder {
    static D3DContext Create();

    static ComPtr<IDXGIFactory> CreateFactory();
    static ComPtr<IDXGIAdapter> PickAdapter(IDXGIFactory*);

    static ComPtr<ID3D12Device> CreateDevice(IDXGIAdapter*);

    static ComPtr<ID3D12CommandQueue> CreateCmdQueue(ID3D12Device*);
};
