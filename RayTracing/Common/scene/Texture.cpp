#include "Texture.h"

#include "core/error.h"

CRT_BEGIN

TextureType fromStringToTextureType(const std::string& type) {
	if (type == "albedo") return TextureType::Albedo;
	if (type == "edges") return TextureType::Edges;
	if (type == "checker") return TextureType::Checker;
	if (type == "bitmap") return TextureType::Bitmap;
	CRT_ERROR("Unknown texture type passed");
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

