#pragma once

#include <dxgi1_6.h>
#include <d3d12.h>
//--------------
#include <wrl.h>
//---------------
#include <vector>
#include <array>
//--------------
#include <log.h>

using Microsoft::WRL::ComPtr;

struct Vertex {
	float x = 0.f;
	float y = 0.f;
};
