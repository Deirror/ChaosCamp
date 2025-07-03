#pragma once

#include <cstdio>
#include <cstdlib>

#define CRT_ERROR(msg) \
	do { \
		fprintf(stderr, "[CRT] %s:%d:%s(): %s\n", __FILE__, __LINE__, __func__, msg); \
		std::abort(); \
	} while (0)

#define CRT_ENSURE(cond, msg) \
	do { \
		if (!(cond)) { \
			CRT_ERROR(msg); \
		} \
	} while (0)
