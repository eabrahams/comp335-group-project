#include "worst_fit.h"

#include <cstdint>
#include <limits>

#include "system_config.h"
#include "job_info.h"

constexpr unsigned WORST_FIT_AVAIL_TIME_THRESHOLD = 100000; // TODO: adjust this to something sensible

void worst_fit(socket_client *client) noexcept(true) {
	system_config *config = parse_config("system.xml");

	client_send(client, "QUIT");
	free_config(config);
};

server_info *find_server(const system_config *config, const job_info &job) noexcept(true) {
	int worst_fit, alt_fit, ini_fit;
	server_info *worst_server, *alt_server, *ini_server;
	worst_fit = alt_fit = ini_fit = std::numeric_limits<int>::min();
	for(auto i = 0; i < config->num_servers; ++i) {
		server_info *server = config->servers + i;
		if(job.can_run(server->avail_resc)) {
			int fitness = job.fitness(server->avail_resc);
			if(fitness > worst_fit && server->state == server_state::SS_IDLE) {
				worst_fit = fitness;
				worst_server = server;
			} else if(fitness > alt_fit && server->avail_time <= WORST_FIT_AVAIL_TIME_THRESHOLD) {
				alt_fit = fitness;
				alt_server = server;
			}
		}
		if(job.can_run(server->type->max_resc)) {
			int fitness = job.fitness(server->type->max_resc);
			if(fitness > ini_fit && server->state == server_state::SS_IDLE) {
				ini_fit = fitness;
				ini_server = server;
			}
		}
	}
	if(worst_fit > 0) {
		return worst_server;
	} else if(alt_fit > 0) {
		return alt_server;
	} else {
		return ini_server;
	}
}