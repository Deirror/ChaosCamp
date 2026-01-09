#pragma once

#include <d3d_context.h>
#include <frame_context.h>
#include <fence_context.h>
#include <readback_context.h>
#include <swapchain_context.h>
#include <vertex_buff_context.h>
#include <raster_pipeline_context.h>
#include <raytracing_pipeline_context.h>
#include <raytracing_output_context.h>
#include <raytracing_dispatch_context.h>
#include <cdxr_debug.h>

// C++ DirectX Rendering
namespace cdxr { 

struct GPUVendorInfo {
    std::wstring description;
    uint32_t vendorId;
    size_t dedicatedVideoMemory;
};

class CDXRenderer {
public:
    struct FrameData {
        int frameIdx = 0;
        int colorIdx = 0;
        float offsX = 0.f;
        float offsY = 0.f;
    };

    enum RenderMode {
        Rasterized,
        RayTraced
    };

public:
    void init(HWND handle);

    void render(const FrameData& data);
    void stopRendering() { flushGPU(); }

    void resize(int width, int height);

    void setClearColor(const std::array<float, 4>& color);

    void updateRenderMode();

public:
    std::vector<GPUVendorInfo> getVendorsInfo();
    void testClearAndSetRT();

private:
    void beginFrame();

    void execute(bool present = true);

    void flushGPU();

    void writeImageToFile();
    void endFrame();

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

    static constexpr int vertexCount = 6;
    std::array<Vertex, vertexCount> vertexData;

    RenderMode mode = RenderMode::Rasterized;

private:
    D3DContext d3d;

    FrameContext frame;
    FenceContext fence;

    SwapChainContext swpCh;

    ReadbackContext rbb;

    VertexBufferContext vb;

    RasterPipelineContext rasterPipe;

    RayTracingOutputContext  rtOut;    
    RayTracingPipelineContext rtPipe; 
    RayTracingDispatchContext rtDisp;

};

}
