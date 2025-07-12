#pragma once

#include "core/crt.h"
#include "third-party/json.hpp"

CRT_BEGIN

class Camera;
class Mesh; 
class Light;

struct Color;
class Vec3;
class Resolution;
class Mat3;
class Material;
class Texture;

struct TriangleIndices;
struct Settings;

class JsonParser {
public:
	JsonParser(const std::string& filename) { parseFile(filename);	}

	void parseFile(const std::string& filename);

	void parseSettings(Settings& settings);
	void parseCamera(Camera& camera);

	void parseLights(std::vector<Light>& lights);
	void parseMeshes(std::vector<Mesh>& meshes);

	void parseMaterials(std::vector<Material>& materials);
	void parseTextures(std::vector<Texture>& textures);

private:
	nlohmann::json jsonData_;
};

CRT_END

void from_json(const nlohmann::json& j, crt::Vec3& vec);
void from_json(const nlohmann::json& j, crt::Color& color);
void from_json(const nlohmann::json& j, crt::Resolution& res);
void from_json(const nlohmann::json& j, crt::TriangleIndices& indices);
void from_json(const nlohmann::json& j, crt::Mesh& mesh);
void from_json(const nlohmann::json& j, crt::Light& light);
void from_json(const nlohmann::json& j, crt::Material& material);
void from_json(const nlohmann::json& j, crt::Texture& texture);
void from_json(const nlohmann::json& j, crt::Mat3& mat);

