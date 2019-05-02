#pragma once
#ifndef system_config_h_
#define system_config_h_

#include "resource_info.h"

#ifdef __cplusplus
#include <cstddef>
extern "C" {
#else
#define noexcept
#include <stdbool.h>
#endif

typedef struct server_type {
	char *name; // the human-readable name of the server type
	unsigned limit; // the maximum number of concurrent instances of this type
	unsigned bootTime; // how long this type of server takes to start up
	float rate;
	resource_info max_resc; // the resources offered by this server type
} server_type;

typedef enum server_state {
	SS_INACTIVE, // server is offline
	SS_BOOTING, // server is starting up, but hasn't finished yet
	SS_IDLE, // server is running, but not doing anything
	SS_ACTIVE, // server is running, and currently doing something
	SS_UNAVAILABLE // server is otherwise unavailable
} server_state;

typedef struct server_info {
	const server_type *type; // the type of this server
	int id; // the type-unique identifier of this server
	server_state state; // the current state of this server
	unsigned avail_time;
	resource_info avail_resc; // the available resources on this server
#ifdef __cplusplus
	bool update(const server_state &state, const unsigned &time, const resource_info &resc) noexcept;
	void reset() noexcept;
#endif
} server_info;

typedef struct system_config {
	const server_type *types; // collection of types, ordered as parsed from XML
	size_t num_types; // number of types
	server_info *servers; // flat collection of servers, ordered by type then id
	size_t num_servers; // number of servers
} system_config;

/*
parses an XML file located at `path`, either returning a pointer
to a valid system_config on success, or returning nullptr and
logging to stderr on failure.
*/
system_config *parse_config(const char *path) noexcept;

// utility to enable a system_config to be freed in one go
void free_config(system_config *config) noexcept;

// gets the server_type matching a given name
const server_type *type_by_name(const system_config *config, const char* name) noexcept;

// gets the first server of a given type, all other servers of that type follow it in the same memory region
server_info *servers_by_type(const system_config *config, const server_type *type) noexcept;

// validates the new resources for the server, returning true and updating if they're valid; false otherwise
bool update_server(server_info *server, server_state state, unsigned time, resource_info resc) noexcept;

// reset the resources availiable on a server to default values
void reset_server(server_info *server) noexcept;

#ifdef __cplusplus
}
#else
#undef noexcept
#endif

#endif
