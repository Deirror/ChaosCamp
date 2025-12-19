#pragma once

#include <cdxr_incl.h>

namespace dapp::re {

class RenderEngine {
public:
	void init(HWND handle);

	void render() { renderer->render(frameData); ++frameData.frameIdx; }
	void stopRendering() { renderer->stopRendering(); }

	void renderRedAndGreen();

	void resize(int width, int height) { renderer->resize(width, height); }

	void setClearColor(const std::array<float, 4>& color) { renderer->setClearColor(color); }

public:
	int getFrameIdx() const { return frameData.frameIdx; }
	void updateColorChoice() { frameData.colorIdx = (frameData.colorIdx + 1) % 2; }

	void updateOffsFrameData(float offsX, float offsY);

private:
	std::unique_ptr<cdxr::CDXRenderer> renderer;

	cdxr::CDXRenderer::FrameData frameData;

};

}
