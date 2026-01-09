#include "d3d_context.h"

D3DContext D3DBuilder::Create() {

	D3DContext d3d;

	d3d.factory = D3DBuilder::CreateFactory();
	d3d.adapter = D3DBuilder::PickAdapter(d3d.factory.Get());

	d3d.device = D3DBuilder::CreateDevice(d3d.adapter.Get());

	d3d.cmdQueue = D3DBuilder::CreateCmdQueue(d3d.device.Get());

	return d3d;
}

ComPtr<IDXGIFactory> D3DBuilder::CreateFactory() {

	ComPtr<IDXGIFactory> factory;
	if (FAILED(CreateDXGIFactory2(0, 
		IID_PPV_ARGS(&factory)))) {

		LOG_FATAL("Unable to create DXGI Factory.");
	}

	return factory;
}

ComPtr<IDXGIAdapter> D3DBuilder::PickAdapter(IDXGIFactory* factory) {

	// Supports adapter selection by gpu preference.
	ComPtr<IDXGIFactory6> factory6 = QueryAs<IDXGIFactory6>(factory); 

	ComPtr<IDXGIAdapter> adapter;
	if (FAILED(factory6->EnumAdapterByGpuPreference(0,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(&adapter)))) {

		LOG_FATAL("Unable to create Adapter.");
	}

	return adapter;
}

ComPtr<ID3D12Device> D3DBuilder::CreateDevice(IDXGIAdapter* adapter) {

	ComPtr<ID3D12Device> device;
	if (FAILED(D3D12CreateDevice(
		adapter,
		D3D_FEATURE_LEVEL_12_0,
		IID_PPV_ARGS(&device)))) {

		LOG_FATAL("Unable to create Device.");
	}

	return device;
}

ComPtr<ID3D12CommandQueue> D3DBuilder::CreateCmdQueue(ID3D12Device* device) {

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	ComPtr<ID3D12CommandQueue> cmdQueue;
	if (FAILED(device->CreateCommandQueue(
		&cmdQueueDesc,
		IID_PPV_ARGS(&cmdQueue)))) {

		LOG_FATAL("Unable to create Cmd Queue.");
	}

	return cmdQueue;
}
