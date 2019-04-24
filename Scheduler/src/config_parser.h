#ifndef config_parser_h_
#define config_parser_h_
#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

	typedef struct server_type {
		char *const name;
		int limit;
		int bootTime;
		float rate;
		int cores;
		int memory;
		int disk;
	} server_type;

	typedef struct system_config {
		server_type * server_types;
		size_t num_of_types;
	} system_config;

	system_config *parse_config(const char *path);

#ifdef __cplusplus
}
#endif

#endif