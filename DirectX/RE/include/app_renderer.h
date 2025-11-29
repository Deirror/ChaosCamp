#pragma once

#include <cdxr_incl.h>

namespace dapp::re {

class RenderEngine {
public:
	void init(HWND handle);

	void render() { renderer->render(); }
	void stopRendering() { renderer->stopRendering(); }

	void renderRedAndGreen();

	void resize(int width, int height) { renderer->resize(width, height); }

	void setClearColor(const std::array<float, 4>& color) { renderer->setClearColor(color); }

public:
	int getFrameIdx() const { return frameIdx; }

private:
	std::unique_ptr<cdxr::CDXRenderer> renderer;

	int frameIdx = 0;
};

}
