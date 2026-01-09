#include "raytracing_dispatch_context.h"

static UINT AlignedSize(UINT size, UINT alignBytes) {
	
	return alignBytes * (size / alignBytes + (size % alignBytes ? 1 : 0));
}

//void copySBTDataToUploadHeap(
//	UINT rayGenOffs, UINT missOffs, UINT hitGroupOffs, 
//	void* rayGenID, void* missID, void* hitGroupID
//) {
//
//	uint8_t* pData = nullptr;
//	sbtUploadBuff->Map(0, nullptr, reinterpret_cast<void**>(&pData));
//
//	memcpy(pData + rayGenOffs, rayGenID, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
//	memcpy(pData + missOffs, missID, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
//	memcpy(pData + hitGroupOffs, hitGroupID, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
//
//	sbtUploadBuff->Unmap(0, nullptr);
//}

static void CopySBTDataToDefaultHeap(FrameContext& frame,
	ID3D12Resource* sbtDefault,	ID3D12Resource* sbtUpload) {

	frame.Reset();

	frame.cmdList->CopyResource(sbtDefault, sbtUpload);

	Transition(
		sbtDefault, 
		D3D12_RESOURCE_STATE_COPY_DEST, 
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, 
		frame.cmdList.Get()
	);

	frame.Close();
}

RayTracingDispatchContext RayTracingDispatchBuilder::Create(
	FrameContext& frame, 
	ID3D12StateObjectProperties* props,
	ID3D12Device* device,
	UINT width,
	UINT height
) {

	RayTracingDispatchContext rtDispCtx;

	void* rayGenID = props->GetShaderIdentifier(L"rayGen");
	if (!rayGenID) {

		LOG_FATAL("Unable to find rayGen identifier.");
	}

	const UINT shaderIDSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
	const UINT recordSize = AlignedSize(shaderIDSize, D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT);
	const UINT sbtSize = recordSize; 

	rtDispCtx.sbtUpload = CreateSBTBuff(
		device, 
		D3D12_HEAP_TYPE_UPLOAD, 
		D3D12_RESOURCE_STATE_GENERIC_READ, 
		sbtSize
	);

	rtDispCtx.sbtDefault = CreateSBTBuff(
		device, 
		D3D12_HEAP_TYPE_DEFAULT, 
		D3D12_RESOURCE_STATE_COPY_DEST, 
		sbtSize
	);

	uint8_t* pData = nullptr;
	if (FAILED(rtDispCtx.sbtUpload->Map(0, nullptr, reinterpret_cast<void**>(&pData)))) {

		LOG_FATAL("Unable to map SBT upload.");
	}

	memcpy(pData, rayGenID, shaderIDSize);
	rtDispCtx.sbtUpload->Unmap(0, nullptr);

	CopySBTDataToDefaultHeap(frame, 
		rtDispCtx.sbtDefault.Get(), 
		rtDispCtx.sbtUpload.Get());

	rtDispCtx.desc = PrepareDispatchRayDesc(
		rtDispCtx.sbtDefault.Get(), 
		recordSize, 
		width, 
		height
	);

	return rtDispCtx;
}

ComPtr<ID3D12Resource> RayTracingDispatchBuilder::CreateSBTBuff(
	ID3D12Device* device, 
	D3D12_HEAP_TYPE heapType, 
	D3D12_RESOURCE_STATES state, 
	UINT size
) {

	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = heapType;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProps.CreationNodeMask = 1;
	heapProps.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC sbtDesc{};
	sbtDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	sbtDesc.Alignment = 0;
	sbtDesc.Width = size;
	sbtDesc.Height = 1;
	sbtDesc.DepthOrArraySize = 1;
	sbtDesc.MipLevels = 1;
	sbtDesc.Format = DXGI_FORMAT_UNKNOWN;
	sbtDesc.SampleDesc.Count = 1;
	sbtDesc.SampleDesc.Quality = 0;
	sbtDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	sbtDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ComPtr<ID3D12Resource> sbtBuff;
	if (FAILED(device->CreateCommittedResource(&heapProps, 
		D3D12_HEAP_FLAG_NONE, &sbtDesc, state,
		nullptr, IID_PPV_ARGS(&sbtBuff)))) {

		LOG_FATAL("Unable to crate SBT Buff.");
	}

	return sbtBuff;
}

D3D12_DISPATCH_RAYS_DESC RayTracingDispatchBuilder::PrepareDispatchRayDesc(
	ID3D12Resource* sbtDefault,
	UINT recordSize, 
	UINT width, 
	UINT height
) {

	D3D12_DISPATCH_RAYS_DESC rayDesc = {};

	rayDesc.RayGenerationShaderRecord.StartAddress = sbtDefault->GetGPUVirtualAddress();
	rayDesc.RayGenerationShaderRecord.SizeInBytes = recordSize;

	rayDesc.Width = width;
	rayDesc.Height = height;
	rayDesc.Depth = 1;

	rayDesc.MissShaderTable = {};
	rayDesc.HitGroupTable = {};
	rayDesc.CallableShaderTable = {};

	return rayDesc;
}
