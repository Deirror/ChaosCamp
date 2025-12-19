#pragma once

#include <cdxr_debug.h>
#include <core.h>

// C++ DirectX Rendering
namespace cdxr { 

struct GPUVendorInfo {
    std::wstring description;
    uint32_t vendorId;
    size_t dedicatedVideoMemory;
};

class CDXRenderer {
public:
    void init(HWND handle);

    void render();
    void stopRendering() { waitForGPU(); }

    void resize(int width, int height);

    void setClearColor(const std::array<float, 4>& color);

public:
    std::vector<GPUVendorInfo> getVendorsInfo();
    void testClearAndSetRT();

private:
    void createFactoryAndAdapters();
    void createDevice();

    void createCmdInterfaces();

    void createFence();

    void createSwapChain(HWND handle);

    void createRTVHeap();
    void createRTVHandles();

    void createRBBHandle();

    void createVertexBuffer();

    void createRootSignature();
    void createPipelineState();

    void createViewport();

private:
    void beginFrame();
    void copyResult(ID3D12Resource* dstRes, ID3D12Resource* cpyRes);

    void close();
    void execute();

    void waitForGPU();

    void writeImageToFile();
    void endFrame();

private:
    void transition(ID3D12Resource* res, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

private:
    static constexpr int RGBA_COLOR_CHANNELS_COUNT = 4;

    static constexpr int defaultWidth = 1280;
    static constexpr int defaultHeight = 720;

    static constexpr int frameCount = 2;
    
private:
#ifdef _DEBUG
    CDXDebugLayer debugLayer;
#endif

    std::array<float, 4> clearColor = { 0., 0., 0., 0. };

    int width = defaultWidth;
    int height = defaultHeight;

private:
    ComPtr<IDXGIFactory7> factory;

    ComPtr<IDXGIAdapter4> adapter;
    ComPtr<ID3D12Device14> device;

    ComPtr<ID3D12CommandQueue> cmdQueue;
    ComPtr<ID3D12CommandAllocator> cmdAlloc;
    ComPtr<ID3D12GraphicsCommandList7> cmdList;

    ComPtr<ID3D12Fence> fence;
    HANDLE fenceEvent = nullptr;
    uint64_t fenceIdx = 0;

    ComPtr<IDXGISwapChain3> swapChain;
    ComPtr<ID3D12Resource2> buffers[frameCount];
    size_t currBuffIdx = 0;

    ComPtr<ID3D12DescriptorHeap> rtvHeap;
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[frameCount] = {};

	ComPtr<ID3D12Resource2> rbbHandle;
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT rtFootprint = {};

	ComPtr<ID3D12Resource2> vertexBuff;
    D3D12_VERTEX_BUFFER_VIEW vertexBuffView = {};

	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12PipelineState> pipelineState;

    D3D12_VIEWPORT viewport = {};
    D3D12_RECT scissorRect = {};

};

}
