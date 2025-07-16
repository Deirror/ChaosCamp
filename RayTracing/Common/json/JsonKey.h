#pragma once

#include "core/crt.h"

CRT_BEGIN

struct JsonKey {
	static constexpr char const* SETTINGS = "settings";
	struct Settings {
		static constexpr char const* BACKGROUND_COLOR = "background_color";
		static constexpr char const* IMAGE_SETTINGS = "image_settings";
		struct Image {
			static constexpr char const* WIDTH = "width";
			static constexpr char const* HEIGHT = "height";
			static constexpr char const* BUCKET_SIZE = "bucket_size"; 
		};
	};

	static constexpr char const* CAMERA = "camera";
	struct Camera {
		static constexpr char const* MATRIX = "matrix";
		static constexpr char const* POSITION = "position";
	};

	static constexpr char const* LIGHTS = "lights";
	struct Lights {
		static constexpr char const* INTENSITY = "intensity";
		static constexpr char const* POSITION = "position";
	};

	static constexpr char const* OBJECTS = "objects";
	struct Objects {
		static constexpr char const* MATERIAL_INDEX = "material_index";
		static constexpr char const* VERTICES = "vertices";
		static constexpr char const* TRIANGLES = "triangles"; 
		static constexpr char const* UVS = "uvs"; 
	};

	static constexpr char const* MATERIALS = "materials";
	struct Material {
		static constexpr char const* TYPE = "type";
		static constexpr char const* ALBEDO = "albedo";
		static constexpr char const* SMOOTH_SHADING = "smooth_shading";
		static constexpr char const* IOR = "ior";
	};

	static constexpr char const* TEXTURES = "textures";
	struct Texture {
		static constexpr char const* NAME = "name";
		static constexpr char const* TYPE = "type";
		static constexpr char const* ALBEDO = "albedo";
		static constexpr char const* EDGE_COLOR= "edge_color";
		static constexpr char const* INNER_COLOR = "inner_color";
		static constexpr char const* EDGE_WIDTH = "edge_width";
		static constexpr char const* COLOR_A = "color_A";
		static constexpr char const* COLOR_B = "color_B";
		static constexpr char const* SQUARE_SIZE = "square_size";
		static constexpr char const* FILE_PATH= "file_path";
	};
};

CRT_END
