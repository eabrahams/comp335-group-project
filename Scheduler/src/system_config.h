#ifndef system_config_h_
#define system_config_h_

#ifdef __cplusplus
extern "C" {
#else
#define noexcept(BOOL)
#include <stdbool.h>
#endif

typedef struct server_type {
	char *name; // the human-readable name of the server type
	unsigned limit; // the maximum number of concurrent instances of this type
	unsigned bootTime; // how long this type of server takes to start up
	float rate;
	unsigned cores; // how many CPU cores this server type offers
	unsigned memory; // how much memory (in MB) this server type offers
	unsigned disk; // how much disk space (in MB) this server type offers
} server_type;

typedef enum server_state {
	SS_OFFLINE, // server is offline
	SS_STARTING, // server is starting up, but hasn't finished yet
	SS_IDLE, // server is running, but not doing anything
	SS_ACTIVE, // server is running, and currently doing something
	SS_UNAVAILABLE // server is otherwise unavailable
} server_state;

typedef struct server_info {
	const server_type *const type; // the type of this server
	const int id; // the type-unique identifier of this server
	server_state state; // the current state of this server
	unsigned avail_time;
	unsigned avail_cores; // how many CPU cores this server has available
	unsigned avail_memory; // how much memory this server has available
	unsigned avail_disk; // how much disk space this server has available
} server_info;

typedef struct system_config {
	const server_type *const types;
	const size_t num_types;
	server_info *const servers;
	const size_t num_servers;
} system_config;

/*
parses an XML file located at `path`, either returning a pointer
to a valid system_config on success, or returning nullptr and
logging to stderr on failure.
*/
system_config *parse_config(const char *path) noexcept(true);

// utility to enable a system_config to be freed in one go
void free_config(system_config *config) noexcept(true);

// gets the server_type matching a given name
const server_type *type_by_name(const system_config *config, const char* name) noexcept(true);

// gets the first server of a given type, all other servers of that type follow it in the same memory region
server_info *servers_by_type(const system_config *config, const server_type *type) noexcept(true);

// validates the new resources for the server, returning true and updating if they're valid; false otherwise
bool update_server(server_info *server, server_state state, unsigned time, unsigned cores, unsigned memory, unsigned disk) noexcept(true);

// reset the resources availiable on a server to default values
void reset_server(server_info *server) noexcept(true);

#ifdef __cplusplus
}
#else
#undef noexcept
#endif

#endif