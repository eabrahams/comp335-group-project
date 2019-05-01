#pragma once
#ifndef job_info_h_
#define job_info_h_

typedef struct {
	unsigned submit_time;
	const int id;
	unsigned runtime;
	unsigned cores;
	unsigned memory;
	unsigned disk;
} job_info;

#endif