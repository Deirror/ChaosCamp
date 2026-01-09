#pragma once

#include <core.h>

struct RasterPipelineContext {
    void Resize(UINT width, UINT height);

    ComPtr<ID3D12RootSignature> rootSignature;
    ComPtr<ID3D12PipelineState> pipelineState;

    D3D12_VIEWPORT viewport = {};
    D3D12_RECT scissor = {};
};

struct RasterPipelineBuilder {
    static RasterPipelineContext Create(
        ID3D12Device* device,
        UINT width,
        UINT height
    );

    static D3D12_VIEWPORT CreateViewport(
        UINT width,
        UINT height
    );

    static D3D12_RECT CreateRect(
        UINT width,
        UINT height
    );

    static ComPtr<ID3D12RootSignature> CreateRootSignature(
        ID3D12Device*
    );

    static ComPtr<ID3D12PipelineState> CreatePipelineState(
        ID3D12Device*,
		ID3D12RootSignature*
    );
};
