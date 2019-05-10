#ifndef stringhelper_h_
#define stringhelper_h_

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include "job_info.h"

#define JOB_REGEX "JOBN ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+)"

typedef struct {
	pcre2_code *re;
	pcre2_match_data *match_data;
} regex_info;

regex_info *regex_init(const char *pattern);
job_info strtojob(const char *jobstr, regex_info *regex);
char *create_schd_str(unsigned long id, char *server_name, unsigned long server_id);

#endif
