#include "Albedo.h"

#include "scene/Texture.h"
#include "HitRecord.h"

CRT_BEGIN

Vec3 albedo(const Texture& texture, const HitRecord& hitRecord) {
	Vec3 color;

	switch (texture.textureType()) {
	case TextureType::Albedo: {
		const AlbedoTextureData& data = texture.albedoTextureData();
		color = data.albedo;
		break;
	}
	case TextureType::Edges: {
		const EdgesTextureData& data = texture.edgesTextureData();
		if (hitRecord.barycentricCoords.x() < data.edgeWidth ||
			hitRecord.barycentricCoords.y() < data.edgeWidth ||
			hitRecord.barycentricCoords.z() < data.edgeWidth) {
			color = data.edgeColor;
		}
		else {
			color = data.innerColor;
		}
		break;
	}
	case TextureType::Checker: {
		const CheckerTextureData& data = texture.checkerTextureData();

		unsigned int x = static_cast<int>(std::floor(hitRecord.puv.x() / data.squareSize));
		unsigned int y = static_cast<int>(std::floor(hitRecord.puv.y() / data.squareSize));

		bool useA = (x + y) % 2 == 0;
		color = useA ? data.colorA : data.colorB;
		break;
	}

	case TextureType::Bitmap: {
		const BitmapTextureData& data = texture.bitmapTextureData();
		unsigned int rowIdx = hitRecord.puv.x() * data.buffer.resolution().width();
		unsigned int colIdx = hitRecord.puv.y() * data.buffer.resolution().height();
		color = data.buffer.at(rowIdx, colIdx);
		break;
	}
	}

	return color;
}

CRT_END

/*
Zebra formula
	bool start = 0;
	unsigned int times = (hitRecord.puv.x() / data.squareSize) + (hitRecord.puv.y() / data.squareSize);
	while (times > 0) {
		start = !start;
		--times;
	}
	if (!start) {
		color = data.colorA;
	}
	else {
		color = data.colorB;
	}
*/
