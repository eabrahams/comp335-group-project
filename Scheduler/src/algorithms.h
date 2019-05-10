#ifndef all_to_largest_h_
#define all_to_largest_h_

#include "socket_client.h"
#include "system_config.h"
#include "job_info.h"

void run_algorithm(socket_client*, server_info*(*alg)(system_config*,server_group*,job_info));
server_info *all_to_largest(system_config*, server_group*, job_info);
server_info *first_fit(system_config*, server_group*, job_info);
server_info *best_fit(system_config*, server_group*, job_info);
extern server_info *worst_fit(system_config*, server_group*, job_info);

#endif
