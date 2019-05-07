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

constexpr unsigned WORST_FIT_AVAIL_TIME_THRESHOLD = 1000; // TODO: adjust this to something sensible

server_info *find_server(const std::vector<server_info*> &servers, const job_info &job) {
	int worst_fit, alt_fit, ini_fit;
	server_info *worst_server, *alt_server, *ini_server;
	worst_fit = alt_fit = ini_fit = std::numeric_limits<int>::min();
	for(auto server : servers) {
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

	if(worst_fit > 0) return worst_server;
	else if(alt_fit > 0) return alt_server;
	else return ini_server;
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
			server_info *server = find_server(candidates, job);

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

