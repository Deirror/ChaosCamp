#include "frame_context.h"

void FrameContext::Reset() {

	if (FAILED(cmdAlloc->Reset())) {

		LOG_FATAL("Unable to reset Cmd Alloc.");
	}

	if (FAILED(cmdList->Reset(cmdAlloc.Get(), nullptr))) {

		LOG_FATAL("Unable to reset Cmd List.");
	}
}

void FrameContext::Close() {

	if (FAILED(cmdList->Close())) {

		LOG_FATAL("Unable to close Cmd List.");
	}
}

FrameContext FrameBuilder::Create(ID3D12Device* device) {

	FrameContext frame;

	frame.cmdAlloc = CreateCmdAlloc(device);
	frame.cmdList = CreateCmdList(device, frame.cmdAlloc.Get());

	return frame;
}

ComPtr<ID3D12CommandAllocator> FrameBuilder::CreateCmdAlloc(ID3D12Device* device) {

	ComPtr<ID3D12CommandAllocator> cmdAlloc;
	if (FAILED(device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAlloc)))) {

		LOG_FATAL("Unable to create Cmd Allocator.");
	}

	return cmdAlloc;
}

ComPtr<ID3D12GraphicsCommandList> FrameBuilder::CreateCmdList(
	ID3D12Device* device, 
	ID3D12CommandAllocator* cmdAlloc
) {

	ComPtr<ID3D12GraphicsCommandList> cmdList;
	if (FAILED(device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAlloc,
		nullptr,
		IID_PPV_ARGS(&cmdList)))) {

		LOG_FATAL("Unable to create Cmd List.");
	}

	// Important, since 'CreateCmdList' creates an opened list.
	cmdList->Close(); 

	return cmdList;
}
