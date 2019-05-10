#include "worst_fit.h"
#include "cpp_util.h"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <limits>
#include <sstream>

constexpr int WAIT_THRESHOLD = 100;//std::numeric_limits<int>::max(); // ten minutes

server_info *worst_fit(system_config* config, server_group *candidates, job_info job) {
	int worst_fit, other_fit, type_fit;
	server_info *worst_server, *other_server, *type_server;
	worst_fit = other_fit = type_fit = std::numeric_limits<int>::min();
	
	// need to handle case where there's a "better" server that isn't in candidates because it isn't really better

	for(auto s = 0; s < config->num_servers; ++s) { //candidates? or config?
		auto server = &config->servers[s];
		if(!job.can_run(server->avail_resc)) continue;	
		if(server->state == server_state::SS_UNAVAILABLE) continue;
		int fitness = job.fitness(server->avail_resc);
		if(fitness > worst_fit && server->avail_time <= job.submit_time && (server->state == SS_ACTIVE || server->state == SS_IDLE)) {
			worst_fit = fitness;
			worst_server = server;
		} else if(fitness > other_fit && server->avail_time <= job.submit_time + WAIT_THRESHOLD) {
			other_fit = fitness;
			other_server = server;
		}
	}

	if(worst_fit >= 0) return worst_server;
	
	if(other_fit >= 0) return other_server;

	for(auto s = 0; s < config->num_servers; ++s) {
		auto server = &config->servers[s];
		if(!job.can_run(server->type->max_resc)) continue;
		if(server->state == SS_UNAVAILABLE) continue;
		int fitness = job.fitness(server->type->max_resc);
		if(fitness > type_fit) {
			type_fit = fitness;
			type_server = server;
		}
	}

	return type_server;
}
