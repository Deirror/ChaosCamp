#include "Render.h"
#include "ImageBuffer.h"

void task1() {
	crt::Scene scene("Scenes/scene2.crtscene");

	crt::Render render(crt::RenderMode::Parallel);
	crt::ImageBuffer buffer = render.render(scene);

	buffer.save("task1_image.ppm", crt::PPMMode::P6);
}
