#include "worst_fit.h"
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

server_info *worst_server(const system_config* config, const std::vector<server_info*> &candidates, const job_info &job) {
	int worst_fit, other_fit, type_fit;
	server_info *worst_server, *other_server, *type_server;
	unsigned other_avail_time, type_avail_time;
	worst_fit = other_fit = type_fit = std::numeric_limits<int>::min();
	other_avail_time = type_avail_time = WORST_FIT_AVAIL_TIME_THRESHOLD;

	for(auto server : candidates) {
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

void worst_fit(socket_client *client) noexcept {
	system_config *config = parse_config("system.xml");
	client_send(client, "REDY");
	std::string response = strcpy_and_free(client_receive(client));
	while(response != "NONE") {
		job_info job = job_from_string(response.c_str());

		//TODO: look at what happens when the server isn't available (ie: all are taken)
		//TODO: schedule more jobs when jobs are finished, maybe add loop to keep querying server to update server states
		try {
			std::vector<server_info*> candidates = config->update(client, job.req_resc);
			server_info *server = worst_server(config, candidates, job);

			std::ostringstream schedule("SCHD ");
			schedule << job.id << " " << server->type->name << " " << server->id;

			if(!client_msg_resp(client, schedule.str().c_str(), "OK")) {
				throw std::runtime_error("Server did not respond as expected to SCHD command!");
			}
		} catch(std::runtime_error err) {
			// also handles errors from the candidate updating
			std::cerr << err.what() << "\n";
			std::cerr << "Scheduler: could not schedule job for " << job.id << ", caused by previous error\n";
		}

		client_send(client, "REDY");
		response = strcpy_and_free(client_receive(client));
	}
	client_send(client, "QUIT");
	free_config(config);
};

