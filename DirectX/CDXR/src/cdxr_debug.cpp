#include "cdxr_debug.h"

namespace cdxr {

CDXDebugLayer::CDXDebugLayer() {
#ifdef _DEBUG

	if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&d3d12Debug)))) {

		LOG_FATAL("Unable to get D3D12 Debug Interface.");
	}

	d3d12Debug->EnableDebugLayer();

	if (FAILED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug)))) {

		LOG_FATAL("Unable to get DXGI Debug Interface.");
	}

	dxgiDebug->EnableLeakTrackingForThread();

#endif
}

CDXDebugLayer::~CDXDebugLayer() {
#ifdef _DEBUG

	if (!dxgiDebug) {

		LOG_FATAL("Cannot report living device objects due to DXGI Debug being nullptr.");
	}

	OutputDebugStringW(L"DXGI Reports living device objects:\n");
	dxgiDebug->ReportLiveObjects(
		DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL)
	);

#endif
}

}