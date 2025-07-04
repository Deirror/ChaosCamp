#include "Render.h"

void task1() {
	crt::Scene scene("Scenes/scene3.crtscene");

	crt::Render render(crt::RenderMode::Parallel);
	crt::ImageBuffer buffer = render.render(scene);

	buffer.save("task1_image.ppm", crt::PPMMode::P6);
}