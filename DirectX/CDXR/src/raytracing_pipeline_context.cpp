#include "raytracing_pipeline_context.h"

static ComPtr<IDxcBlob> CompileShader(
	const std::wstring& filename,
	const std::wstring& entryPoint,
	const std::wstring& target
) {

	ComPtr<IDxcLibrary> library;
	ComPtr<IDxcCompiler> compiler;
	ComPtr<IDxcBlobEncoding> source;
	ComPtr<IDxcOperationResult> result;

	if (FAILED(DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library)))) {

		LOG_FATAL("Unable to create DXC library.");
	}

	if (FAILED(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler)))) {

		LOG_FATAL("Unable to create DXC compiler.");
	}

	if (FAILED(library->CreateBlobFromFile(filename.c_str(), nullptr, &source))) {

		LOG_FATAL("Unable to load shader file.");
	}

	const wchar_t* args[] = {
		L"-Zi",
		L"-Qembed_debug",
		L"-Od",
		L"-Zpr"
	};

	if (FAILED(compiler->Compile(source.Get(),
		filename.c_str(), entryPoint.c_str(), target.c_str(),
		args, _countof(args), nullptr, 0, nullptr, &result))) {

		LOG_FATAL("Unable to compile shader.");
	}

	HRESULT status = S_OK;
	result->GetStatus(&status);
	if (FAILED(status)) {

		ComPtr<IDxcBlobEncoding> errors;
		result->GetErrorBuffer(&errors);

		if (errors && errors->GetBufferSize()) {

			LOG_FATAL((const char*)errors->GetBufferPointer());
		}

		LOG_FATAL("DXC compile failed.");
	}

	ComPtr<IDxcBlob> shader;
	if (FAILED(result->GetResult(&shader))) {

		ComPtr<IDxcBlobEncoding> errors;
		result->GetErrorBuffer(&errors);

		if (errors) {

			LOG_FATAL(reinterpret_cast<const char*>(errors->GetBufferPointer()));
		}

		LOG_FATAL("Shader compilation error.");
	}

	return shader;
}

D3D12_STATE_SUBOBJECT RayTracingPipelineBuilder::CreateLibSubobject(
	RayTracingPipelineContext::LibSubobject& libSubobj, 
	const wchar_t* entryPoint
) {

	ComPtr<IDxcBlob> blob = CompileShader(L"hlsl/RayTracing.hlsl", entryPoint, L"lib_6_5");

	libSubobj.blob = blob;

	libSubobj.exprt = {};
	libSubobj.exprt.Name = entryPoint;
	libSubobj.exprt.Flags = D3D12_EXPORT_FLAG_NONE;

	libSubobj.lib = {};
	libSubobj.lib.DXILLibrary.BytecodeLength = blob->GetBufferSize();
	libSubobj.lib.DXILLibrary.pShaderBytecode = blob->GetBufferPointer();
	libSubobj.lib.NumExports = 1;
	libSubobj.lib.pExports = &libSubobj.exprt;

	D3D12_STATE_SUBOBJECT libSubobject{};
	libSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
	libSubobject.pDesc = &libSubobj.lib;

	return libSubobject;
}

D3D12_STATE_SUBOBJECT RayTracingPipelineBuilder::CreateShaderConfigSubobject(
	D3D12_RAYTRACING_SHADER_CONFIG& shaderCfg
) {

	shaderCfg.MaxPayloadSizeInBytes = 0; // no TraceRay => no payload needed
	shaderCfg.MaxAttributeSizeInBytes = 0; // no hit shaders => no attributes needed

	D3D12_STATE_SUBOBJECT sub{};
	sub.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
	sub.pDesc = &shaderCfg;

	return sub;
}

D3D12_STATE_SUBOBJECT RayTracingPipelineBuilder::CreatePipelineConfigSubobject(
	D3D12_RAYTRACING_PIPELINE_CONFIG& pipeCfg
) {

	pipeCfg .MaxTraceRecursionDepth = 1;

	D3D12_STATE_SUBOBJECT pipelineConfigSubobject{};
	pipelineConfigSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
	pipelineConfigSubobject.pDesc = &pipeCfg;

	return pipelineConfigSubobject;
}

D3D12_STATE_SUBOBJECT RayTracingPipelineBuilder::CreateGlobalRootSignatureSubobject(
	D3D12_GLOBAL_ROOT_SIGNATURE& globalRootSigDesc,
	ID3D12RootSignature* globalRootSig
) {

	globalRootSigDesc.pGlobalRootSignature = globalRootSig;

	D3D12_STATE_SUBOBJECT rootSignSubobject{};
	rootSignSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
	rootSignSubobject.pDesc = &globalRootSigDesc;

	return rootSignSubobject;
}

RayTracingPipelineContext RayTracingPipelineBuilder::Create(
	ID3D12Device* device
) {

	RayTracingPipelineContext rtPipeCtx;

	rtPipeCtx.globalRootSig = CreateGlobalRootSignature(device);

	std::array<D3D12_STATE_SUBOBJECT, 4> subobjs = {
		CreateLibSubobject(rtPipeCtx.rayGenLibSubobj, L"rayGen"),
		//CreateLibSubobject(rtPipeCtx.missLibSubobj, L"miss")
		//CreateLibSubobject(rtPipeCtx.closestHitLibSubobj, L"closestHit")
		CreateShaderConfigSubobject(rtPipeCtx.shaderCfg),
		CreatePipelineConfigSubobject(rtPipeCtx.pipeCfg),
		CreateGlobalRootSignatureSubobject(rtPipeCtx.globalRootSigDesc, rtPipeCtx.globalRootSig.Get()),
		//createHitGroupLibSubobject()
	};

	rtPipeCtx.stateObj = CreateStateObj(device, subobjs);
	
	return rtPipeCtx;
}

ComPtr<ID3D12RootSignature> RayTracingPipelineBuilder::CreateGlobalRootSignature(
	ID3D12Device* device
) {

	D3D12_DESCRIPTOR_RANGE range{};
	range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	range.NumDescriptors = 1;
	range.BaseShaderRegister = 0; // u0
	range.RegisterSpace = 0;
	range.OffsetInDescriptorsFromTableStart = 0;

	D3D12_ROOT_PARAMETER param{};
	param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	param.DescriptorTable.NumDescriptorRanges = 1;
	param.DescriptorTable.pDescriptorRanges = &range;

	D3D12_ROOT_SIGNATURE_DESC desc{};
	desc.NumParameters = 1;
	desc.pParameters = &param;
	desc.NumStaticSamplers = 0;
	desc.pStaticSamplers = nullptr;
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

	ComPtr<ID3DBlob> blob, error;
	if (FAILED(D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error))) {
		if (error) LOG_FATAL((const char*)error->GetBufferPointer());
		LOG_FATAL("SerializeRootSignature failed");
	}

	ComPtr<ID3D12RootSignature> globalRootSign;
	if (FAILED(device->CreateRootSignature(
		0, blob->GetBufferPointer(), blob->GetBufferSize(),
		IID_PPV_ARGS(&globalRootSign)))) {
		LOG_FATAL("CreateRootSignature failed");
	}

	return globalRootSign;
}

ComPtr<ID3D12StateObject> RayTracingPipelineBuilder::CreateStateObj(
	ID3D12Device* device,
	const std::array<D3D12_STATE_SUBOBJECT, 4>& subobjs
) {

	D3D12_STATE_OBJECT_DESC rtPsoDesc{};
	rtPsoDesc.Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;
	rtPsoDesc.NumSubobjects = (UINT)subobjs.size();
	rtPsoDesc.pSubobjects = subobjs.data();

	ComPtr<ID3D12StateObject> rtStateObj;
	if (FAILED(QueryAs<ID3D12Device5>(device)->CreateStateObject(&rtPsoDesc, IID_PPV_ARGS(&rtStateObj)))) {

		LOG_FATAL("Unable to create RayTraced State Obj");
	}

	return rtStateObj;
}

//D3D12_STATE_SUBOBJECT createHitGroupLibSubobject() {
//
//	hitGroupDesc.HitGroupExport = L"HitGroup";
//	hitGroupDesc.Type = D3D12_HIT_GROUP_TYPE_TRIANGLES;
//	hitGroupDesc.ClosestHitShaderImport = L"closestHit";
//
//	D3D12_STATE_SUBOBJECT hitGroupSubobject{};
//	hitGroupSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
//	hitGroupSubobject.pDesc = &hitGroupDesc;
//
//	return hitGroupSubobject;
//}

