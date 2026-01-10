#include "vertex_buff_context.h"

#include <d3dx12.h>

VertexBufferContext VertexBufferBuilder::Create(
    ID3D12Device* device,
    ID3D12GraphicsCommandList* cmdList,
    void* vertexData, 
    UINT vertexCount, 
    UINT stride
) {

    VertexBufferContext vb{};
    vb.sizeBytes = vertexCount * stride;

// === Create Buffers === //
    vb.defaultBuff = CreateBuffer(
        device,
        vb.sizeBytes,
        D3D12_RESOURCE_FLAG_NONE,
        D3D12_RESOURCE_STATE_COMMON,
        D3D12_HEAP_TYPE_DEFAULT
    );

    vb.uploadBuff = CreateBuffer(
        device,
        vb.sizeBytes,
        D3D12_RESOURCE_FLAG_NONE,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        D3D12_HEAP_TYPE_UPLOAD
    );

// === Upload Vertex Data === //
    UploadDataToBuff(
        vb.uploadBuff.Get(),
        vertexData,
        vb.sizeBytes
    );

// === Copy To Default Heap === //
    Transition(
        vb.defaultBuff.Get(),
        D3D12_RESOURCE_STATE_COMMON,
        D3D12_RESOURCE_STATE_COPY_DEST,
        cmdList
    );

    cmdList->CopyBufferRegion(
        vb.defaultBuff.Get(), 0,
        vb.uploadBuff.Get(), 0,
        vb.sizeBytes
    );

    Transition(
        vb.defaultBuff.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        cmdList
    );

// === Create Buff View For Input Assembler === //
    vb.view = CreateVertexBufferView(
        vb.defaultBuff.Get(),
        stride,
        vertexCount
    );

    return vb;
}

D3D12_VERTEX_BUFFER_VIEW VertexBufferBuilder::CreateVertexBufferView(
	ID3D12Resource* buff, 
    UINT stride,
    UINT vertexCount
) {

	D3D12_VERTEX_BUFFER_VIEW vertexBuffView = D3D12_VERTEX_BUFFER_VIEW{};
	vertexBuffView.BufferLocation = buff->GetGPUVirtualAddress();
	vertexBuffView.StrideInBytes = stride;
	vertexBuffView.SizeInBytes = vertexCount * stride; 

	return vertexBuffView;
}
