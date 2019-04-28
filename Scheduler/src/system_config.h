#ifndef system_config_h_
#define system_config_h_
#ifdef __cplusplus
extern "C" {
#else
#define noexcept(BOOL)
#include <stdbool.h>
#endif

// TODO: make all members of server_type constant
struct server_info;

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
	const server_type *const types;
	const size_t num_types;
	server_info *const servers;
	const size_t num_servers;
} system_config;

system_config *parse_config(const char *path) noexcept(true);
void release_config(system_config *config) noexcept(true);

server_info *server_of_type(const system_config *const config, const server_type *const type) noexcept(true);

void reset_server(server_info *const server) noexcept(true);



#ifdef __cplusplus
}
#endif

#endif