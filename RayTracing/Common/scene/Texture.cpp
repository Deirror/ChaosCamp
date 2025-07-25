#include "Texture.h"

#include "core/error.h"

CRT_BEGIN

TextureType fromStringToTextureType(const std::string& type) {
	if (type == "albedo") return TextureType::Albedo;
	if (type == "edges") return TextureType::Edges;
	if (type == "checker") return TextureType::Checker;
	if (type == "zebra") return TextureType::Zebra;
	if (type == "bitmap") return TextureType::Bitmap;
	CRT_ERROR("Unknown texture type passed");
}

Vec3 Texture::albedo(const Vec3& coords) const {
	Vec3 color;

	switch (textureType_) {
	case TextureType::Albedo: {
		const AlbedoTextureData& data = albedoTextureData();
		color = data.albedo;
		break;
	}
	case TextureType::Edges: {
		const EdgesTextureData& data = edgesTextureData();
		if (coords.x() < data.edgeWidth ||
			coords.y() < data.edgeWidth ||
			coords.z() < data.edgeWidth) {
			color = data.edgeColor;
		}
		else {
			color = data.innerColor;
		}
		break;
	}
	case TextureType::Checker: {
		const CheckerTextureData& data = checkerTextureData();

		int x = static_cast<int>(std::floor(coords.x() / data.squareSize));
		int y = static_cast<int>(std::floor(coords.y() / data.squareSize));

		bool useA = (x + y) % 2 == 0;
		color = useA ? data.colorA : data.colorB;
		break;
	}
	case TextureType::Zebra: {
		const ZebraTextureData& data = zebraTextureData();

		bool start = 0;
		int times = static_cast<int>((coords.x() / data.lineHeight) + (coords.y() / data.lineHeight));
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

		break;
	}
	case TextureType::Bitmap: {
		const BitmapTextureData& data = bitmapTextureData();

		int rowIdx = static_cast<int>(math::min(coords.x() * data.buffer.resolution().width(), data.buffer.resolution().width() - 1.f));
		int colIdx = static_cast<int>(math::min(coords.y() * data.buffer.resolution().height(), data.buffer.resolution().height() - 1.f));

		color = data.buffer.at(rowIdx, colIdx);
		break;
	}
	}

	return color;
}

void Texture::textureType(TextureType textureType) {
	textureType_ = textureType;
	if (textureType_ == TextureType::Bitmap) {
		if (!bitmapTextureData_.has_value()) {
			bitmapTextureData_.emplace();
		}
	}
	else {
		bitmapTextureData_.reset();
	}
}

const AlbedoTextureData& Texture::albedoTextureData() const {
	CRT_ENSURE(textureType_ == TextureType::Albedo, "Texture type is not albedo");
	return textureData_.albedo;
}
void Texture::albedoTextureData(const AlbedoTextureData& data) {
	CRT_ENSURE(textureType_ == TextureType::Albedo, "Texture type is not albedo");
	textureData_.albedo = data;
}

const EdgesTextureData& Texture::edgesTextureData() const {
	CRT_ENSURE(textureType_ == TextureType::Edges, "Texture type is not edges");
	return textureData_.edges;
}

void Texture::edgesTextureData(const EdgesTextureData& data) {
	CRT_ENSURE(textureType_ == TextureType::Edges, "Texture type is not edges");
	textureData_.edges = data;
}

const CheckerTextureData& Texture::checkerTextureData() const {
	CRT_ENSURE(textureType_ == TextureType::Checker, "Texture type is not checker");
	return textureData_.checker;
}

void Texture::checkerTextureData(const CheckerTextureData& data) {
	CRT_ENSURE(textureType_ == TextureType::Checker, "Texture type is not checker");
	textureData_.checker = data;
}

const ZebraTextureData& Texture::zebraTextureData() const {
	CRT_ENSURE(textureType_ == TextureType::Zebra, "Texture type is not zebra");
	return textureData_.zebra;
}

void Texture::zebraTextureData(const ZebraTextureData& data) {
	CRT_ENSURE(textureType_ == TextureType::Zebra, "Texture type is not zebra");
	textureData_.zebra = data;
}

const BitmapTextureData& Texture::bitmapTextureData() const {
	CRT_ENSURE(textureType_ == TextureType::Bitmap, "Texture type is not bitmap");
	CRT_ENSURE(bitmapTextureData_.has_value(), "BitmapTextureData is not set");
	return bitmapTextureData_.value();
}

void Texture::bitmapTextureData(const BitmapTextureData& data) {
	CRT_ENSURE(textureType_ == TextureType::Bitmap, "Texture type is not bitmap");
	bitmapTextureData_ = data;
}

CRT_END
