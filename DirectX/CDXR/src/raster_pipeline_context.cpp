#include "raster_pipeline_context.h"

#include "TriangleVS.hlsl.h" // To include the file, hit compile on the .hlsl file in the dir.
#include "Triangle.hlsl.h" // To include the file, hit compile on the .hlsl file in the dir.

void RasterPipelineContext::Resize(UINT width, UINT height) {

	viewport = RasterPipelineBuilder::CreateViewport(width, height);
	scissor = RasterPipelineBuilder::CreateRect(width, height);
}

RasterPipelineContext RasterPipelineBuilder::Create(
	ID3D12Device* device, 
	UINT width, 
	UINT height
) {

	RasterPipelineContext rpCtx;

	rpCtx.viewport = CreateViewport(width, height);
	rpCtx.scissor = CreateRect(width, height);

	rpCtx.rootSignature = CreateRootSignature(device);

	rpCtx.pipelineState = CreatePipelineState(
		device, 
		rpCtx.rootSignature.Get()
	);

	return rpCtx;
}

D3D12_VIEWPORT RasterPipelineBuilder::CreateViewport(UINT width, UINT height) {

	D3D12_VIEWPORT viewport;

	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

    return viewport;
}

D3D12_RECT RasterPipelineBuilder::CreateRect(UINT width, UINT height) {

	D3D12_RECT scissorRect;

	scissorRect.top = 0;
	scissorRect.left = 0;
	scissorRect.right = width;
	scissorRect.bottom = height;

	return scissorRect;
}

ComPtr<ID3D12RootSignature> RasterPipelineBuilder::CreateRootSignature(ID3D12Device* device) {

	CD3DX12_ROOT_PARAMETER1 rootParam;
	rootParam.InitAsConstants(4, 0, 0, D3D12_SHADER_VISIBILITY_ALL);

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignDesc{};
	rootSignDesc.Init_1_1(1, &rootParam, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;

	if (FAILED(D3D12SerializeVersionedRootSignature(
		&rootSignDesc,
		&signature,
		&error))) {

		LOG_FATAL("Unable to serialize Root Signature.");
	}

	ComPtr<ID3D12RootSignature> rootSignature;
	if (FAILED(device->CreateRootSignature(0,
		signature->GetBufferPointer(),
		signature->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)))) {

		LOG_FATAL("Unable to create Root Signature.");
	}

	return rootSignature;
}

ComPtr<ID3D12PipelineState> RasterPipelineBuilder::CreatePipelineState(ID3D12Device* device, ID3D12RootSignature* rootSignature) {

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "PART_ID", 0, DXGI_FORMAT_R32_UINT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	psoDesc.pRootSignature = rootSignature;
	psoDesc.PS = { g_triangle, _countof(g_triangle) };
	psoDesc.VS = { g_trianglevs, _countof(g_trianglevs) };
	psoDesc.InputLayout = { inputLayout, _countof(inputLayout)};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = false;
	psoDesc.DepthStencilState.StencilEnable = false;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;

	ComPtr<ID3D12PipelineState> pipelineState;
	if (FAILED(QueryAs<ID3D12Device1>(device)->CreateGraphicsPipelineState(
		&psoDesc, IID_PPV_ARGS(&pipelineState)))) {

		LOG_FATAL("Unable to create Graphics Pipeline.");
	}

	return pipelineState;
}
