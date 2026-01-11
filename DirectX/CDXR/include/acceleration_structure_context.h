#pragma once

#include "core.h"

struct AccelerationStructure {
	ComPtr<ID3D12Resource> result;       
	ComPtr<ID3D12Resource> scratch;      
	ComPtr<ID3D12Resource> instanceDesc; 
};

struct AccelerationStructureContext {
    AccelerationStructure blas;
    AccelerationStructure tlas;
};

struct AccelerationStructureBuilder {
	static AccelerationStructureContext Create(
		ID3D12Device*,
		ID3D12GraphicsCommandList*,
		ID3D12Resource* vertexBuffer,
		UINT vertexCount,
		UINT vertexStride
	);

	static AccelerationStructure CreateBLAS(
		ID3D12Device5*,
		ID3D12GraphicsCommandList4*,
		ID3D12Resource* vertexBuffer,
		UINT vertexCount,
		UINT vertexStride
	);

	static AccelerationStructure CreateTLAS(
		ID3D12Device5*,
		ID3D12GraphicsCommandList4*,
		const std::vector<AccelerationStructure>& blasList,
		const std::vector<std::array<float, 12>>& transforms
	);
};
