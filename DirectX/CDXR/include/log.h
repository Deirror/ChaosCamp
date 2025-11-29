#pragma once

#include <print>

#define LOG_ERROR(msg)										     \
    do {                                                         \
		std::println("[DXRT] {}:{}:{}(): {}",					 \
					 __FILE__, __LINE__, __func__, msg);		 \
    } while (0)

#define LOG_FATAL(msg)										     \
    do {                                                         \
		LOG_ERROR(msg);											 \
		std::abort();											 \
    } while (0)