#include "acceleration_structure_context.h"

AccelerationStructureContext AccelerationStructureBuilder::Create(
	ID3D12Device* device, 
	ID3D12GraphicsCommandList* cmdList, 
	ID3D12Resource* vertexBuffer, 
	UINT vertexCount, 
	UINT vertexStride
) {

	AccelerationStructureContext accStCtx;

	ComPtr<ID3D12Device5> device5 = QueryAs<ID3D12Device5>(device);
	ComPtr<ID3D12GraphicsCommandList4> cmdList4 = QueryAs<ID3D12GraphicsCommandList4>(cmdList);

	accStCtx.blas = CreateBLAS(
		device5.Get(),
		cmdList4.Get(),
		vertexBuffer,
		vertexCount,
		vertexStride
	);
	accStCtx.tlas = CreateTLAS(
		device5.Get(),
		cmdList4.Get(),
		{ accStCtx.blas },
		{ DefaultTransform() }
	);

	return accStCtx;
}

AccelerationStructure AccelerationStructureBuilder::CreateBLAS(
	ID3D12Device5* device,
	ID3D12GraphicsCommandList4* cmdList,
	ID3D12Resource* vertexBuffer,
	UINT vertexCount,
	UINT vertexStride
) {

	AccelerationStructure blas{};

	D3D12_RAYTRACING_GEOMETRY_DESC geomDesc{};
	geomDesc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
	geomDesc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;

	geomDesc.Triangles.VertexBuffer.StartAddress = vertexBuffer->GetGPUVirtualAddress();
	geomDesc.Triangles.VertexBuffer.StrideInBytes = vertexStride;
	geomDesc.Triangles.VertexCount = vertexCount;
	geomDesc.Triangles.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT; 
	geomDesc.Triangles.IndexBuffer = 0;
	geomDesc.Triangles.IndexCount = 0;

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs{};
	inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
	inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.NumDescs = 1;
	inputs.pGeometryDescs = &geomDesc;
	inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
	device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &info);

	const UINT64 scratchSize = AlignUp(info.ScratchDataSizeInBytes,
		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT);
	const UINT64 resultSize = AlignUp(info.ResultDataMaxSizeInBytes,
		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT);

	blas.scratch = CreateBuffer(
		device,
		info.ScratchDataSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, 
		D3D12_HEAP_TYPE_DEFAULT
	);

	blas.result = CreateBuffer(
		device,
		info.ResultDataMaxSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, 
		D3D12_HEAP_TYPE_DEFAULT
	);

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC buildDesc{};
	buildDesc.Inputs = inputs;
	buildDesc.ScratchAccelerationStructureData = blas.scratch->GetGPUVirtualAddress();
	buildDesc.DestAccelerationStructureData = blas.result->GetGPUVirtualAddress();

	cmdList->BuildRaytracingAccelerationStructure(&buildDesc, 0, nullptr);

	D3D12_RESOURCE_BARRIER uavBarrier = {};
	uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.UAV.pResource = blas.result.Get();
	cmdList->ResourceBarrier(1, &uavBarrier);

	return blas;
}

AccelerationStructure AccelerationStructureBuilder::CreateTLAS(
	ID3D12Device5* device,
	ID3D12GraphicsCommandList4* cmdList,
	const std::vector<AccelerationStructure>& blasList,
	const std::vector<std::array<float, 12>>& transforms
) {

	AccelerationStructure tlas{};

	UINT instanceCount = static_cast<UINT>(blasList.size());
	if (instanceCount == 0) {

		LOG_FATAL("CreateTLAS called with zero instances.");
	}

	std::vector<D3D12_RAYTRACING_INSTANCE_DESC> instances(instanceCount);

	for (UINT i = 0; i < instanceCount; ++i) {

		D3D12_RAYTRACING_INSTANCE_DESC& inst = instances[i];
		memset(&inst, 0, sizeof(float) * 12);

		inst.InstanceMask = 0xFFu;
		inst.InstanceID = i;
		inst.InstanceContributionToHitGroupIndex = 0;
		inst.Flags = D3D12_RAYTRACING_INSTANCE_FLAG_NONE;

		memcpy(inst.Transform, transforms[i].data(), sizeof(float) * 12);

		inst.AccelerationStructure = blasList[i].result->GetGPUVirtualAddress();
	}

	size_t instanceDescsSize = sizeof(D3D12_RAYTRACING_INSTANCE_DESC) * instanceCount;
	tlas.instanceDesc = CreateBuffer(device,
		instanceDescsSize,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_HEAP_TYPE_UPLOAD);

	UploadDataToBuff(tlas.instanceDesc.Get(), instances.data(), instanceDescsSize);

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs{};
	inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
	inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.NumDescs = instanceCount;
	inputs.InstanceDescs = tlas.instanceDesc->GetGPUVirtualAddress(); 
	inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
	device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &info);

	const UINT64 scratchSize = AlignUp(info.ScratchDataSizeInBytes,
		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT);
	const UINT64 resultSize = AlignUp(info.ResultDataMaxSizeInBytes,
		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BYTE_ALIGNMENT);

	tlas.scratch = CreateBuffer(
		device,
		info.ScratchDataSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, 
		D3D12_HEAP_TYPE_DEFAULT
	);

	tlas.result = CreateBuffer(
		device,
		info.ResultDataMaxSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, 
		D3D12_HEAP_TYPE_DEFAULT
	);

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC buildDesc{};
	buildDesc.Inputs = inputs;
	buildDesc.ScratchAccelerationStructureData = tlas.scratch->GetGPUVirtualAddress();
	buildDesc.DestAccelerationStructureData = tlas.result->GetGPUVirtualAddress();

	cmdList->BuildRaytracingAccelerationStructure(&buildDesc, 0, nullptr);

	D3D12_RESOURCE_BARRIER uavBarrier = {};
	uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.UAV.pResource = tlas.result.Get();
	cmdList->ResourceBarrier(1, &uavBarrier);

	return tlas;
}
