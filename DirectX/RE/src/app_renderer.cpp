#include <app_renderer.h>

namespace dapp::re {

void RenderEngine::init(HWND handle) {

	renderer = std::make_unique<cdxr::CDXRenderer>();
	renderer.get()->init(handle);
}

void re::RenderEngine::renderRedAndGreen() {

	float frameCoef = static_cast<float>(frameIdx % 1000) / 1000.f;

	setClearColor({ frameCoef, 1.f - frameCoef, 0.f, 1.f });
	renderer->render(); 
		
	++frameIdx;
}

}