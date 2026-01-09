#include "fence_context.h"

void FenceContext::Signal(ID3D12CommandQueue* cmdQueue, UINT64& outFrameVal) {

	outFrameVal = nextVal;
	cmdQueue->Signal(fence.Get(), nextVal++);
}

void FenceContext::Wait(UINT64 val) {

	if (fence->GetCompletedValue() >= val) {
		return;
	}

	if (FAILED(fence->SetEventOnCompletion(val, event))) {

		LOG_FATAL("Unable to set Event on completion.");
	}

	WaitForSingleObject(event, INFINITE);
}

FenceContext FenceBuilder::Create(ID3D12Device* device) {

	FenceContext fence;

	fence.fence = FenceBuilder::CreateFence(device);
	fence.event = FenceBuilder::CreateFenceEvent();

	return fence;
}

ComPtr<ID3D12Fence> FenceBuilder::CreateFence(ID3D12Device* device) {

	ComPtr<ID3D12Fence> fence;
	if (FAILED(device->CreateFence(0, 
		D3D12_FENCE_FLAG_NONE, 
		IID_PPV_ARGS(&fence)))) {

		LOG_FATAL("Unable to create Fence.");
	}

	return fence;
}

HANDLE FenceBuilder::CreateFenceEvent() {

	HANDLE event = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	if (!event) {

		LOG_FATAL("Unable to create Event.");
	}

	return event;
}