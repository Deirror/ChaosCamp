#pragma once

#include "core.h"

struct VertexBufferContext {
    ComPtr<ID3D12Resource> defaultBuff;
    ComPtr<ID3D12Resource> uploadBuff;  
    D3D12_VERTEX_BUFFER_VIEW view = {};
    UINT sizeBytes = 0;
};

struct VertexBufferBuilder {
    static VertexBufferContext Create(
        ID3D12Device*,
		ID3D12GraphicsCommandList*,
        void* vertexData,
        UINT vertexCount,
        UINT stride
    );

    static D3D12_VERTEX_BUFFER_VIEW CreateVertexBufferView(
		ID3D12Resource*,
		UINT vertexCount, 
		UINT stride
    );
};
