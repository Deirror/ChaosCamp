#pragma once

#include <core.h>

struct ReadbackContext {
    ComPtr<ID3D12Resource> buff;
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT footPrint = {};
};

struct ReadbackBuilder {
    static ReadbackContext Create(
        ID3D12Device*,
        ID3D12Resource*
    );

    static D3D12_PLACED_SUBRESOURCE_FOOTPRINT CreateFootPrint(
		ID3D12Device*, 
		ID3D12Resource*,
		UINT64& width
    );

    static ComPtr<ID3D12Resource> CreateBuffer(
		ID3D12Device*, 
        UINT64 width
    );
};
