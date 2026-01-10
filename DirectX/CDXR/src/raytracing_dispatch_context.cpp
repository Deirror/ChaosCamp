#include "raytracing_dispatch_context.h"

static UINT AlignedSize(UINT size, UINT alignBytes) {
	
	return alignBytes * (size / alignBytes + (size % alignBytes ? 1 : 0));
}

static void* GetShaderIdentifier(
	ID3D12StateObjectProperties* props, 
	const wchar_t* entryPoint
) {

	void* id = props->GetShaderIdentifier(entryPoint);
	if (!id) {

		LOG_FATAL("Unable to find Identifier.");
	}

	return id;
}

static void CopySBTDataToUploadHeap(ID3D12Resource* sbtUpload,
	UINT rayGenOffs, UINT missOffs, UINT hitGroupOffs, 
	void* rayGenID, void* missID, void* hitGroupID) {

	uint8_t* pData = nullptr;
	if (FAILED(sbtUpload->Map(0, nullptr, reinterpret_cast<void**>(&pData)))) {

		LOG_FATAL("Unable to map SBT upload.");
	}

	memcpy(pData + rayGenOffs, rayGenID, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	memcpy(pData + missOffs, missID, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	memcpy(pData + hitGroupOffs, hitGroupID, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);

	sbtUpload->Unmap(0, nullptr);
}

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

void RayTracingDispatchContext::Reset() {

	sbtUpload.Reset();
	sbtDefault.Reset();
	desc = {};
}

void RayTracingDispatchContext::UpdateDimensions(UINT width, UINT height) {

	desc.Width = width;
	desc.Height = height;
}

RayTracingDispatchContext RayTracingDispatchBuilder::Create(
	FrameContext& frame,
	ID3D12StateObjectProperties* props,
	ID3D12Device* device,
	UINT width,
	UINT height
) {

	RayTracingDispatchContext rtDispCtx{};

// === Get Identifiers === //
	void* rayGenID = GetShaderIdentifier(props, L"rayGen");
	void* missID = GetShaderIdentifier(props, L"miss");
	void* hitGroupID = GetShaderIdentifier(props, L"HitGroup");

// === Calc Alignment Per Shader === //
	const UINT shaderIdSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES; 

	const UINT recordAlign = D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT; 
	const UINT tableAlign = D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT;  

	const UINT recordStride = (UINT)AlignUp(shaderIdSize, recordAlign);

	const UINT rayGenOffs = 0;
	const UINT missOffs = (UINT)AlignUp((UINT64)rayGenOffs + recordStride, tableAlign);   
	const UINT hitGroupOffs = (UINT)AlignUp((UINT64)missOffs + recordStride, tableAlign);

	const UINT sbtSize = (UINT)AlignUp((UINT64)hitGroupOffs + recordStride, tableAlign);

// === Create Shader-Binding Table Buffers === //
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

// === Copy Data To Heaps === //
	CopySBTDataToUploadHeap(
		rtDispCtx.sbtUpload.Get(),
		rayGenOffs, missOffs, hitGroupOffs,
		rayGenID, missID, hitGroupID
	);

	CopySBTDataToDefaultHeap(
		frame,
		rtDispCtx.sbtDefault.Get(),
		rtDispCtx.sbtUpload.Get()
	);

// === Prep Ray Data For Raytracing Shader === //
	rtDispCtx.desc = PrepareDispatchRayDesc(
		rtDispCtx.sbtDefault.Get(),
		recordStride,
		rayGenOffs,
		missOffs,
		hitGroupOffs,
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
	UINT rayGenOffs, 
	UINT missOffs, 
	UINT hitGroupOffs, 
	UINT width, 
	UINT height
) {

	D3D12_DISPATCH_RAYS_DESC rayDesc = {};

	rayDesc.RayGenerationShaderRecord.StartAddress = sbtDefault->GetGPUVirtualAddress() + rayGenOffs;
	rayDesc.RayGenerationShaderRecord.SizeInBytes = recordSize;

	rayDesc.MissShaderTable.StartAddress = sbtDefault->GetGPUVirtualAddress() + missOffs;
	rayDesc.MissShaderTable.SizeInBytes = recordSize;
	rayDesc.MissShaderTable.StrideInBytes = recordSize;

	rayDesc.HitGroupTable.StartAddress = sbtDefault->GetGPUVirtualAddress() + hitGroupOffs;
	rayDesc.HitGroupTable.SizeInBytes = recordSize;
	rayDesc.HitGroupTable.StrideInBytes = recordSize;

	rayDesc.Width = width;
	rayDesc.Height = height;
	rayDesc.Depth = 1;

	rayDesc.CallableShaderTable = {};

	return rayDesc;
}
