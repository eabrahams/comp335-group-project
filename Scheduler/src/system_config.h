#pragma once
#ifndef system_config_h_
#define system_config_h_

#include "resource_info.h"

#ifdef __cplusplus
#include "cpp_util.h"
extern "C" {
#else
#define noexcept
#include <stdbool.h>
#endif

typedef struct server_info_s server_info;
typedef struct server_group_s server_group;
typedef struct system_config_s system_config;
#include "socket_client.h"

typedef struct server_type {
	char *name; // the human-readable name of the server type
	unsigned limit; // the maximum number of concurrent instances of this type
	unsigned bootTime; // how long this type of server takes to start up
	float rate;
	resource_info max_resc; // the resources offered by this server type
#ifdef __cplusplus
	void release() noexcept;
#endif
} server_type;

typedef enum server_state {
	SS_INACTIVE, // server is offline
	SS_BOOTING, // server is starting up, but hasn't finished yet
	SS_IDLE, // server is running, but not doing anything
	SS_ACTIVE, // server is running, and currently doing something
	SS_UNAVAILABLE // server is otherwise unavailable
} server_state;

struct server_info_s {
	const server_type *type; // the type of this server
	int id; // the type-unique identifier of this server
	server_state state; // the current state of this server
	int avail_time; // can be -1, meaning "available now" (based on RESC example info from spec)
	resource_info avail_resc; // the available resources on this server
#ifdef __cplusplus
	bool update(server_state state, int time, const resource_info &resc) noexcept;
	void reset() noexcept;
#endif
};

// container for an arbitrary collection of servers
struct server_group_s {
	server_info *const * servers; // flat collection, no order
	size_t num_servers; // number of servers
#ifdef __cplusplus
	void release() noexcept;
#endif
};

struct system_config_s {
	const server_type *types; // collection of types, ordered as parsed from XML
	size_t num_types; // number of types
	server_info *servers; // flat collection of servers, ordered by type then id
	size_t num_servers; // number of servers
#ifdef __cplusplus
	// handler for `RESC All`
	void update(socket_client *client);
	// handler for `RESC Type ..`
	void update(socket_client *client, const server_type *type);
	// handler for `RESC Avail ..`, returns the servers updated
	std::vector<server_info*> update(socket_client *client, const resource_info &resc);
	void release() noexcept;
#endif
};

/*
parses an XML file located at `path`, either returning a pointer
to a valid system_config on success, or returning nullptr and
logging to stderr on failure.
*/
system_config *parse_config(const char *path) noexcept;

// utility to enable a system_config to be freed in one go
void free_config(system_config *config) noexcept;

// utility to enable a server_group to be freed in one go
void free_group(server_group *group) noexcept;

// gets the server_type matching a given name
const server_type *type_by_name(const system_config *config, const char* name) noexcept;

// gets the first server of a given type, all other servers of that type follow it in the same memory region
server_info *start_of_type(const system_config *config, const server_type *type) noexcept;

bool update_config(system_config *config, socket_client *client) noexcept;

bool update_servers_by_type(system_config *config, socket_client *client, const server_type *type) noexcept;

server_group *updated_servers_by_avail(system_config *config, socket_client *client, const resource_info resc) noexcept;

// validates the new resources for the server, returning true and updating if they're valid; false otherwise
//bool update_server(server_info *server, server_state state, int time, resource_info resc) noexcept;

// reset the resources availiable on a server to default values
void reset_server(server_info *server) noexcept;

#ifdef __cplusplus
}
#else
#undef noexcept
#endif

#endif
