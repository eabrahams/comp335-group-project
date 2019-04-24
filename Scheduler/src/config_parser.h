#ifndef config_parser_h_
#define config_parser_h_
#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

// TODO: make all members of server_type constant

typedef struct server_type {
	char *name;
	unsigned limit;
	unsigned bootTime;
	float rate;
	unsigned cores;
	unsigned memory;
	unsigned disk;
} server_type;

typedef enum server_state { SS_OFFLINE, SS_STARTING, SS_IDLE, SS_INACTIVE, SS_UNAVAILABLE } server_state;

typedef struct server_info {
	const server_type *const type;
	const int id;
	server_state state;
	unsigned avail_time;
	unsigned avail_cores;
	unsigned avail_memory;
	unsigned avail_disk;
} server_info;

typedef struct system_config {
	server_type *server_types;
	size_t num_of_types;
	server_info *servers;
	size_t num_of_servers;
} system_config;

system_config *parse_config(const char *path);

#ifdef __cplusplus
}
#endif

#endif