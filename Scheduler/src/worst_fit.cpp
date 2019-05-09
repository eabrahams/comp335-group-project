//#include "worst_fit.h"
#include "cpp_util.h"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <limits>
#include <sstream>

#include "system_config.h"
#include "job_info.h"

constexpr unsigned WORST_FIT_AVAIL_TIME_THRESHOLD = 10000; // TODO: adjust this to something sensible

extern "C" {
	server_info *worst_fit(system_config* config, server_group *candidates, job_info job);
}

server_info *worst_fit(system_config* config, server_group *candidates, job_info job) {
	int worst_fit, other_fit, type_fit;
	server_info *worst_server, *other_server, *type_server;
	unsigned other_avail_time, type_avail_time;
	worst_fit = other_fit = type_fit = std::numeric_limits<int>::min();
	other_avail_time = type_avail_time = WORST_FIT_AVAIL_TIME_THRESHOLD;

	for(auto s = 0; s < candidates->num_servers; ++s) {
		auto *server = candidates->servers[s];
		int fitness = job.fitness(server->avail_resc);
		if(fitness > worst_fit && server->state == server_state::SS_IDLE) {
			worst_fit = fitness;
			worst_server = server;
		} else if(fitness >= other_fit && server->avail_time <= other_avail_time) {
			other_fit = fitness;
			other_server = server;
			other_avail_time = server->avail_time;
		}
	}

	if(worst_fit > 0) return worst_server;
	else if(other_fit > 0) return other_server;

	for(auto t = 0; t < config->num_types; ++t) {
		auto type = &config->types[t];
		if(!job.can_run(type->max_resc)) continue;
		server_info *server_offset = start_of_type(config, type);
		for(auto s = 0; s < type->limit; ++s) {
			auto server = &server_offset[s];
			if(server->state == server_state::SS_UNAVAILABLE) continue;
			int fitness = job.fitness(server->avail_resc);
			if(fitness >= type_fit && server->avail_time <= type_avail_time) {
				type_fit = fitness;
				type_server = server;
				type_avail_time = server->avail_time;
			}
		}
	}
	
	return type_server;
}
