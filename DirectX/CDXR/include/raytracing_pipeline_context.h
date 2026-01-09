#pragma once

#include <core.h>

struct RayTracingPipelineContext {
    ComPtr<ID3D12RootSignature> globalRootSig;
    ComPtr<ID3D12StateObject> stateObj;

    struct LibSubobject {
		ComPtr<IDxcBlob> blob;
		D3D12_EXPORT_DESC exprt{};
		D3D12_DXIL_LIBRARY_DESC lib{};
    };

    LibSubobject rayGenLibSubobj;
    LibSubobject missLibSubobj;
    LibSubobject closestHitLibSubobj;

    D3D12_RAYTRACING_SHADER_CONFIG shaderCfg{};
    D3D12_RAYTRACING_PIPELINE_CONFIG pipeCfg{};
    D3D12_GLOBAL_ROOT_SIGNATURE globalRootSigDesc{};

    D3D12_HIT_GROUP_DESC hitGroupDesc = {};
    D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION assocDesc = {};
};

struct RayTracingPipelineBuilder {
    static RayTracingPipelineContext Create(ID3D12Device*);

    static ComPtr<ID3D12RootSignature> CreateGlobalRootSignature(
        ID3D12Device*
    );

    static ComPtr<ID3D12StateObject> CreateStateObj(
		ID3D12Device*,
		const std::array<D3D12_STATE_SUBOBJECT, 8>&
    );

    static D3D12_STATE_SUBOBJECT CreateLibSubobject(
        RayTracingPipelineContext::LibSubobject&,
        const wchar_t* entryPoint
    );

    static D3D12_STATE_SUBOBJECT CreateShaderConfigSubobject(
		D3D12_RAYTRACING_SHADER_CONFIG&
    );

    static D3D12_STATE_SUBOBJECT CreatePipelineConfigSubobject(
		D3D12_RAYTRACING_PIPELINE_CONFIG&
    );

    static D3D12_STATE_SUBOBJECT CreateGlobalRootSignatureSubobject(
		D3D12_GLOBAL_ROOT_SIGNATURE&,
        ID3D12RootSignature*
    );

    static D3D12_STATE_SUBOBJECT CreateExportsAssociation(
		D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION& assocDesc,
        D3D12_STATE_SUBOBJECT*
    );

    static D3D12_STATE_SUBOBJECT CreateHitGroupLibSubobject(
        D3D12_HIT_GROUP_DESC& hitGroupDesc
    );
};
