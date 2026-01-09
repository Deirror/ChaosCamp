#include "readback_context.h"

ReadbackContext ReadbackBuilder::Create(
	ID3D12Device* device, 
	ID3D12Resource* source
) {

	ReadbackContext rbb;

	UINT64 rbbSize = 0;
	rbb.footPrint = CreateFootPrint(device, source, rbbSize);

	rbb.buff = CreateBuffer(device, rbbSize);

	return rbb;
}

D3D12_PLACED_SUBRESOURCE_FOOTPRINT ReadbackBuilder::CreateFootPrint(
	ID3D12Device* device, 
	ID3D12Resource* source,
	UINT64& width 
) {

	D3D12_RESOURCE_DESC desc = source->GetDesc();

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footPrint;
	device->GetCopyableFootprints(
		&desc, 
		0, 1, 0,
		&footPrint,
		nullptr,
		nullptr,
		&width
	);

	return footPrint;
}

ComPtr<ID3D12Resource> ReadbackBuilder::CreateBuffer(
	ID3D12Device* device,
	UINT64 width
) {

	D3D12_RESOURCE_DESC readBackBuff{};
	readBackBuff.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	readBackBuff.Width = width;
	readBackBuff.Height = 1;
	readBackBuff.MipLevels = 1;
	readBackBuff.SampleDesc.Count = 1;
	readBackBuff.DepthOrArraySize = 1;
	readBackBuff.Format = DXGI_FORMAT_UNKNOWN;
	readBackBuff.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_HEAP_PROPERTIES rbbHeapProps{};
	rbbHeapProps.Type = D3D12_HEAP_TYPE_READBACK;

    ComPtr<ID3D12Resource> buffer;
	if (FAILED(device->CreateCommittedResource(
		&rbbHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&readBackBuff,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&buffer)))) {

		LOG_FATAL("Unable to create RBB Resource.");
	}

	return buffer;
}
