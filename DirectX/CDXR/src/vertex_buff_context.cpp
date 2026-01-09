#include "vertex_buff_context.h"

#include <d3dx12.h>

VertexBufferContext VertexBufferBuilder::Create(
    ID3D12Device* device,
    void* vertexData, 
    UINT vertexCount, 
    UINT stride
) {

	VertexBufferContext vb;

	vb.buff = CreateVertexBuffer(device, vertexCount * stride);

	void* mappedData = nullptr;
	if (FAILED(vb.buff->Map(0, nullptr, &mappedData))) {
		LOG_FATAL("Unable to map Vertex Buffer.");
	}

	memcpy(mappedData, vertexData, vertexCount * stride);

	vb.buff->Unmap(0, nullptr);

	vb.view = CreateVertexBufferView(vb.buff.Get(), vertexCount, stride);

	return vb;
}

ComPtr<ID3D12Resource> VertexBufferBuilder::CreateVertexBuffer(
	ID3D12Device* device,
	UINT size
) {

	ComPtr<ID3D12Resource> vertexBuff;

	D3D12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(size);

	if (FAILED(QueryAs<ID3D12Device1>(device)->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuff)))) {

		LOG_FATAL("Unable to create Vertex Buffer.");
	}

	return vertexBuff;
}

D3D12_VERTEX_BUFFER_VIEW VertexBufferBuilder::CreateVertexBufferView(
	ID3D12Resource* buff, 
    UINT vertexCount, 
    UINT stride
) {

	D3D12_VERTEX_BUFFER_VIEW vertexBuffView = D3D12_VERTEX_BUFFER_VIEW{};
	vertexBuffView.BufferLocation = buff->GetGPUVirtualAddress();
	vertexBuffView.StrideInBytes = stride;
	vertexBuffView.SizeInBytes = vertexCount * stride; 

	return vertexBuffView;
}
