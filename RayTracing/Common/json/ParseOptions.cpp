#include "ParseOptions.h"

CRT_BEGIN

ParseOptions& ParseOptions::withOption(const std::string& option) {
	if (!validateOption(option)) {
		CRT_ERROR("Invalid option passed");
	}
	options_.insert(option);

	return *this;
}
	

ParseOptions& ParseOptions::withoutOption(const std::string& option) {
	if (!validateOption(option)) {
		CRT_ERROR("Invalid option passed");
	}
	options_.erase(option);

	return *this;
}

bool ParseOptions::hasOption(const std::string& option) const {
	if (!validateOption(option)) {
		CRT_ERROR("Invalid option passed");
	}
	return options_.count(option) > 0;
}

CRT_END
