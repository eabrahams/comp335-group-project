#define _GNU_SOURCE
#include "stringhelper.h"
#include "job_info.h"

#include <pcre2.h>
#include <stdio.h>
#include <string.h>

char *create_schd_str(unsigned long id, char *server_name, unsigned long server_id) {
	char *schd;
	asprintf(&schd, "%s %lu %s %lu", "SCHD", id, server_name, server_id);
	return schd;
}

regex_info *regex_init(const char *regex_str) {
	regex_info *info = malloc(sizeof *info);

	int errnum;
	PCRE2_SIZE erroff;
	PCRE2_SPTR pattern = (PCRE2_SPTR)regex_str;
	info->re = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, 0, &errnum, &erroff, NULL);
	if (info->re == NULL) {
		PCRE2_UCHAR buffer[256];
		pcre2_get_error_message(errnum, buffer, sizeof(buffer));
		printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroff, buffer);
		free(info);
		return NULL;
	}

	info->match_data = pcre2_match_data_create_from_pattern(info->re, NULL);
	return info;
}

job_info strtojob(const char *jobstr, regex_info *regex) {
	//pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(re, NULL);
	//regex->match_data = pcre2_match_data_create_from_pattern(regex->re, NULL);
	int rc = pcre2_match(regex->re, (PCRE2_SPTR)jobstr, strlen(jobstr), 0, 0, regex->match_data, NULL);
	if (rc < 0) {
		switch(rc) {
			case PCRE2_ERROR_NOMATCH:
				fprintf(stderr, "%s%s\n", "Bad job string: ", jobstr);
				break;
		}

		fprintf(stderr, "%s%d\n", "Matching error: ", rc);
		pcre2_match_data_free(regex->match_data);
		exit(1);
	}

	PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(regex->match_data);
	if (rc == 0) {
		fprintf(stderr, "%s\n", "ovector not big enough for all captured substrings");
	}

	unsigned long values[rc - 1];
	int i;
	size_t substring_len;
	const char *substring;
	char *tmpstr;
	for (i = 1; i < rc; i++) {
		substring = jobstr + ovector[2*i];
		substring_len = ovector[2*i+1] - ovector[2*i];
		tmpstr = calloc(substring_len + 1, sizeof *tmpstr);
		strncpy(tmpstr, substring, substring_len);
		values[i-1] = strtoul(tmpstr, NULL, 0);
		free(tmpstr);
	}

	job_info j;
	j.submit_time = values[0];
	j.id = values[1];
	j.est_runtime = values[2];
	j.req_resc.cores = values[3];
	j.req_resc.memory = values[4];
	j.req_resc.disk = values[5];

	//pcre2_match_data_free(regex->match_data);
	return j;
}

