#ifndef all_to_largest_h_
#define all_to_largest_h_

#include "socket_client.h"
#include "system_config.h"
#include "job_info.h"

typedef enum { ALL_TO_LARGEST, FIRST_FIT, BEST_FIT, WORST_FIT } algorithm_t;

//void run_algorithm(socket_client*, server_info*(*alg)(system_config*,server_group*,job_info));
void run_algorithm(socket_client*, algorithm_t algorithm);
server_info *all_to_largest(system_config*, job_info);
server_info *first_fit(system_config*, job_info);
server_info *best_fit(system_config*, job_info);
extern server_info *worst_fit(system_config*, server_group*, job_info);

#endif
