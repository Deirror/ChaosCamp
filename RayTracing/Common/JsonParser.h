#pragma once

#include "crt.h"
#include "third-party/json.hpp"

CRT_BEGIN

class Settings;
class Camera;
class Mesh; 
class Light;

class Color;
class Vec3;
class Resolution;
class Mat3;

struct TriangleIndices;
struct Settings;

class JsonParser {
public:
	JsonParser(const std::string& filename) { parseFile(filename);	}

	void parseFile(const std::string& filename);

	void parseSettings(Settings& settings);
	void parseCamera(Camera& camera);

	void parseLights(std::vector<Light>& light);

	void parseMeshes(std::vector<Mesh>& meshes);

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
void from_json(const nlohmann::json& j, crt::Mat3& mat);

