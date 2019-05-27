#pragma once
#ifndef system_config_h_
#define system_config_h_

#include "resource_info.h"
#include "job_info.h"

#ifdef __cplusplus
#include "cpp_util.h"
#ifndef EXTERN_C
#define EXTERN_C
#define EXTERN_C_system_config_h_
extern "C" {
#endif
#else
#define noexcept
#include <stdbool.h>
#endif

#include <stdint.h>
#include "socket_client.h"

#include "list.h"

typedef struct server_type {
	char *name; // the human-readable name of the server type
	uintmax_t limit; // the maximum number of concurrent instances of this type
	uintmax_t bootTime; // how long this type of server takes to start up
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

typedef struct server_info {
	const server_type *type; // the type of this server
	size_t id; // the type-unique identifier of this server
	server_state state; // the current state of this server
	intmax_t avail_time; // can be -1, meaning "available now" (based on RESC example info from spec)
	resource_info avail_resc; // the available resources on this server
	struct llist_node *job_id_list;
#ifdef __cplusplus
	bool update(server_state state, intmax_t time, const resource_info &resc) noexcept;
	void reset() noexcept;
#endif
} server_info;

// container for an arbitrary collection of servers
typedef struct server_group {
	server_info *const * servers; // flat collection, no order
	size_t num_servers; // number of servers
#ifdef __cplusplus
	void release() noexcept;
#endif
} server_group;

typedef struct system_config {
	const server_type *types; // collection of types, ordered as parsed from XML
	size_t num_types; // number of types
	server_info *servers; // flat collection of servers, ordered by type then id
	size_t num_servers; // number of servers
	job_info *jobs;
	size_t num_jobs;
#ifdef __cplusplus
	const server_type *type_by_name(const char *name) const;
	server_info *start_of_type(const server_type *type) const;
	// handler for `RESC All`, throws if the server does something unexpected
	void update(socket_client *client);
	// handler for `RESC Type .., throws if the server does something unexpected`
	void update(socket_client *client, const server_type *type);
	// handler for `RESC Avail ..`, throws if the server does something unexpected
	// returns the servers updated by the RESC command
	std::vector<server_info*> update(socket_client *client, const resource_info &resc);
	// format is "<type> <id> <state> <avail_time> <avail_cores> <avail_mem> <avail_disk>"
	server_info *update_from_string(const std::string &str);
	void release() noexcept;
#endif
} system_config;

/*
parses an XML file located at `path`, either returning a pointer
to a valid system_config on success, or returning nullptr and
logging to stderr on failure.
the caller is responsible for calling free_config on the result
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

// wrapper around system_config.update, returns true on success and false on failure
bool update_config(system_config *config, socket_client *client) noexcept;

// wrapper around system_config.update, returns true on success and false on failure
bool update_servers_by_type(system_config *config, socket_client *client, const server_type *type) noexcept;

// wrapper around system_config.update, returns a collection of servers on success and nullptr on failure
// the caller is responsible for calling free_group on the result
server_group *updated_servers_by_avail(system_config *config, socket_client *client, const resource_info resc) noexcept;

// validates the new resources for the server, returning true and updating if they're valid; false otherwise
//bool update_server(server_info *server, server_state state, int time, resource_info resc) noexcept;

// reset the resources availiable on a server to default values
void reset_server(server_info *server) noexcept;

#ifdef __cplusplus
#ifdef EXTERN_C_system_config_h_
}
#undef EXTERN_C_system_config_h_
#undef EXTERN_C
#endif
#else
#undef noexcept
#endif

#endif
