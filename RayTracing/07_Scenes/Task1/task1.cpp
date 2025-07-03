#include "Render.h"

void task1() {
	crt::Scene scene("Scenes/scene4.crtscene");

	crt::Render render;
	render.render(scene);
}
