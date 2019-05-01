#include "worst_fit.h"

#include <cstdint>
#include <limits>

void worst_fit(system_config *config, const job_info *job) noexcept(true) {
	int worst_fit, alt_fit;
	worst_fit = alt_fit = std::numeric_limits<int>::min();
	for(auto i = 0; i < config->num_types; ++i) {
		const server_type *type = config->types + i;
		for(auto j = 0; j < type->limit; ++j) {
			server_info *server = servers_by_type(config, type) + j;
			if(server_can_run_job(server, job)) {

			}
		}
	}
	
}