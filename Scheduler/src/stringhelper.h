#ifndef stringhelper_h_
#define stringhelper_h_

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include "job_info.h"

typedef struct {
	pcre2_code *re;
	pcre2_match_data *match_data;
} regex_info;

regex_info *regex_init(char *pattern);
job_info strtojob(char *jobstr, regex_info *regex);
char *create_schd_str(unsigned int id, char *server_name, int server_id);

#endif
