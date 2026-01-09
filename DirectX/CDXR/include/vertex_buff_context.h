#pragma once

#include <core.h>

struct VertexBufferContext {
    ComPtr<ID3D12Resource> buff;
    D3D12_VERTEX_BUFFER_VIEW view = {};
};

struct VertexBufferBuilder {
    static VertexBufferContext Create(
        ID3D12Device*,
        void* vertexData,
        UINT vertexCount,
        UINT stride
    );

    static ComPtr<ID3D12Resource> CreateVertexBuffer(
		ID3D12Device*,
		UINT size
    );

    static D3D12_VERTEX_BUFFER_VIEW CreateVertexBufferView(
		ID3D12Resource*,
		UINT vertexCount, 
		UINT stride
    );
};
