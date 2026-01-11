#pragma once

#ifdef _DEBUG
#include <d3d12sdklayers.h>
#include <dxgidebug.h>
#endif
//---------------
#include "core.h"

namespace cdxr {

/// Debug Layer, which reports events on app close.
/// Primary usage is to report if there are memory leaks.
class CDXDebugLayer {
public:
	CDXDebugLayer();
	~CDXDebugLayer();

private:
#ifdef _DEBUG
	ComPtr<ID3D12Debug6> d3d12Debug;
	ComPtr<IDXGIDebug1> dxgiDebug;
#endif

};

}
