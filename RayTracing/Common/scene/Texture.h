#pragma once

#include <string>
#include <optional>

#include "math/Vec3.h"
#include "image/BitmapBuffer.h"

CRT_BEGIN

enum class TextureType {
	Albedo,
	Edges,
	Checker,
	Bitmap
};

TextureType fromStringToTextureType(const std::string& type);

struct AlbedoTextureData {
	Vec3 albedo;
};

struct EdgesTextureData {
	Vec3 edgeColor;
	Vec3 innerColor;
	float edgeWidth = 0.f;
};

struct CheckerTextureData {
	Vec3 colorA;
	Vec3 colorB;
	float squareSize = 0.f;
};

struct BitmapTextureData {
	BitmapTextureData() = default;
	BitmapTextureData(const std::string& filepath) : filepath(filepath), buffer(filepath) {}

	std::string filepath;
	BitmapBuffer buffer;
};

union TextureData {
	AlbedoTextureData albedo;
	EdgesTextureData edges;
	CheckerTextureData checker;

	TextureData() : checker() {};
	~TextureData() = default;
};

class Texture {
public:
	Texture() = default;
	Texture(const std::string& name, TextureType textureType) : name_(name), textureType_(textureType) {}

	TextureType textureType() const { return textureType_; }
	void textureType(TextureType textureType);

	const std::string& name() const { return name_; }
	void name(const std::string& name) { name_ = name; }

	const AlbedoTextureData& albedoTextureData() const;
	void albedoTextureData(const AlbedoTextureData& data);

	const EdgesTextureData& edgesTextureData() const;
	void edgesTextureData(const EdgesTextureData& data);

	const CheckerTextureData& checkerTextureData() const;
	void checkerTextureData(const CheckerTextureData& data);

	const BitmapTextureData& bitmapTextureData() const;
	void bitmapTextureData(const BitmapTextureData& data);

private:
	std::string name_;
	TextureType textureType_ = TextureType::Albedo;

	TextureData textureData_;
	std::optional<BitmapTextureData> bitmapTextureData_;
};

CRT_END
