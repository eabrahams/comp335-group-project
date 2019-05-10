#include "worst_fit.h"
#include "cpp_util.h"

#include <limits>
#include <cstdint>

server_info *worst_fit(system_config* config, server_group *candidates, job_info job) {
	intmax_t worst_fit, other_fit, type_fit;
	server_info *worst_server, *other_server, *type_server;
	worst_fit = other_fit = type_fit = std::numeric_limits<intmax_t>::min();

	for(auto s = 0; s < config->num_servers; ++s) {
		auto server = &config->servers[s];

		if(server->state == server_state::SS_UNAVAILABLE) continue;

		else if(job.can_run(server->avail_resc)) {
			intmax_t fitness = job.fitness(server->avail_resc);

			if(fitness > worst_fit && server->avail_time <= static_cast<intmax_t>(job.submit_time) && (server->state == SS_ACTIVE || server->state == SS_IDLE)) {
				worst_fit = fitness;
				worst_server = server;

			} else if(fitness > other_fit && server->avail_time <= static_cast<intmax_t>(job.submit_time + server->type->bootTime)) {
				other_fit = fitness;
				other_server = server;
			}

		} else if(job.can_run(server->type->max_resc)) {
			intmax_t fitness = job.fitness(server->type->max_resc);

			if(fitness > type_fit) {
				type_fit = fitness;
				type_server = server;
			}
		}
	}

	if(worst_fit >= 0) return worst_server;
	
	else if(other_fit >= 0) return other_server;

	else return type_server;
}
