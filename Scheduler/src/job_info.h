#pragma once
#ifndef job_info_h_
#define job_info_h_

#include "resource_info.h"

typedef struct {
	unsigned submit_time;
	const int id;
	unsigned runtime;
	resource_info req_resc;
} job_info;

#endif