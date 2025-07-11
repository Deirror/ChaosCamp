#pragma once

#include <string>
#include <unordered_set>

#include "core/error.h"

#include "JsonKey.h"

CRT_BEGIN

inline std::unordered_set<std::string> ValidParseOptions { 
	JsonKey::CAMERA, JsonKey::OBJECTS, JsonKey::SETTINGS,
	JsonKey::MATERIALS, JsonKey::LIGHTS
};

class ParseOptions {
public:
	ParseOptions() : options_({ JsonKey::CAMERA, JsonKey::OBJECTS, JsonKey::SETTINGS }) {}

	template<typename ...Args>
	ParseOptions(Args&& ...options);

	ParseOptions& withOption(const std::string& option);
	ParseOptions& withoutOption(const std::string& option);

	bool hasOption(const std::string& option) const;

	std::unordered_set<std::string> options() const { return options_; }

private:
	bool validateOption(const std::string& option) const { return ValidParseOptions.count(option) > 0; }

private:
	std::unordered_set<std::string> options_;
};

template<typename ...Args>
inline ParseOptions::ParseOptions(Args&& ...options) {
	for (const auto& option : { options... }) {
		withOption(option);
	}
}

CRT_END


