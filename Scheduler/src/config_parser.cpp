#include <tinyxml.h>
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>
#include <string>

#include "config_parser.h"

system_config *parse_config(const char *path) {
	TiXmlDocument doc;
	if(!doc.LoadFile(path)) return nullptr;
	TiXmlElement *root = doc.RootElement();
	if(strcmp("system", root->Value())) return nullptr;
	TiXmlElement *nodes = root->FirstChildElement();
	if(strcmp("servers", nodes->Value())) return nullptr;
	auto server_types = std::vector<server_type>();
	TiXmlElement *node;
	for(node = nodes->FirstChildElement(); node != nullptr; node = node->NextSiblingElement()) {
		int limit = std::stoi(node->Attribute("limit"));
		if(limit <= 0) {
			fprintf(stderr, "Parser: `limit` must be positive (was %i)\n", limit);
			break;
		}

		int bootTime = std::stoi(node->Attribute("bootupTime"));
		if(bootTime < 0) {
			fprintf(stderr, "Parser: `bootupTime` cannot be negative (was %i)\n", bootTime);
			break;
		}

		float rate = std::stof(node->Attribute("rate"));
		if(rate <= 0.0) {
			fprintf(stderr, "Parser: `rate` must be positive (was %f)\n", rate);
			break;
		}

		int cores = std::stoi(node->Attribute("coreCount"));
		if(cores <= 0) {
			fprintf(stderr, "Parser: `coreCount` must be positive (was %i)\n", cores);
			break;
		}

		int memory = std::stoi(node->Attribute("memory"));
		if(memory <= 0) {
			fprintf(stderr, "Parser: `memory` must be positive (was %i)\n", memory);
			break;
		}

		int disk = std::stoi(node->Attribute("disk"));
		if(disk <= 0) {
			fprintf(stderr, "Parser: `disk` must be positive (was %i)\n", disk);
			break;
		}

		// do this last so we don't have to free it if we broke out above
		const char *name = node->Attribute("name");
		char *name_copy = (char *) malloc(strlen(name) + 1);
		strcpy(name_copy, name);

		server_types.push_back(server_type{ name_copy, limit, bootTime, rate, cores, memory, disk });	
	}
	if(node == nullptr) { // success
		system_config *config = (system_config *) malloc(sizeof(system_config));
		config->num_of_types = server_types.size();
		config->server_types = (server_type *) malloc(sizeof(server_type)*config->num_of_types);
		memcpy(config->server_types, server_types.data(), sizeof(server_type)*config->num_of_types);
		return config;
	} else { // failure, free and return null
		for(auto server : server_types) free(server.name);
		return nullptr;
	}
}
