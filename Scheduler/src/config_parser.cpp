#include <tinyxml.h>
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>
#include <numeric>

#include "config_parser.h"

system_config *parse_config(const char *path) {
	TiXmlDocument doc;
	if(!doc.LoadFile(path)) return nullptr;
	TiXmlElement *root = doc.RootElement();
	if(strcmp("system", root->Value())) {
		fprintf(stderr, "Parser: bad root element: expected 'system', but got '%s'\n", root->Value());
		return nullptr;
	}
	TiXmlElement *nodes = root->FirstChildElement();
	if(strcmp("servers", nodes->Value())) {
		fprintf(stderr, "Parser: bad container element: expected 'servers', but got '%s'\n", nodes->Value());
		return nullptr;
	}
	auto server_types = std::vector<server_type>();
	TiXmlElement *node; // declare outside of loop to check if it finished properly
	for(node = nodes->FirstChildElement(); node != nullptr; node = node->NextSiblingElement()) {
		server_type type;

		if(strcmp("server", node->Value())) {
			fprintf(stderr, "Parser: bad member element: expected 'server', but got '%s'\n", node->Value());
			break;
		}

		if(node->QueryUnsignedAttribute("limit", &type.limit) != TIXML_SUCCESS) {
			fprintf(stderr, "Parser: bad member element: must have positive integer attribute 'limit'\n");
			break;
		} else if(type.limit == 0) {
			fprintf(stderr, "Parser: bad attribute: 'limit' must be a positive integer (was 0)\n");
			break;
		}

		if(node->QueryUnsignedAttribute("bootupTime", &type.bootTime) != TIXML_SUCCESS) {
			fprintf(stderr, "Parser: bad member element: must have non-negative integer attribute 'bootupTime'\n");
			break;
		}

		if(node->QueryFloatAttribute("rate", &type.rate) != TIXML_SUCCESS) {
			fprintf(stderr, "Parser: bad member element: must have positive floating-point attribute 'rate'\n");
			break;
		} else if(type.rate < 0) {
			fprintf(stderr, "Parser: bad attribute: 'rate' must be a positive floating-point number (was %f)\n", type.rate);
			break;
		}

		if(node->QueryUnsignedAttribute("coreCount", &type.cores) != TIXML_SUCCESS) {
			fprintf(stderr, "Parser: bad member element: must have positive integer attribute 'coreCount'\n");
			break;
		} else if(type.cores == 0) {
			fprintf(stderr, "Parser: bad attribute: 'coreCount' must be a positive integer (was %i)\n");
			break;
		}

		if(node->QueryUnsignedAttribute("memory", &type.memory) != TIXML_SUCCESS) {
			fprintf(stderr, "Parser: bad member element: must have positive integer attribute 'memory'\n");
			break;
		} else if(type.memory == 0) {
			fprintf(stderr, "Parser: bad attribute: 'memory' must be a positive integer (was 0)\n");
			break;
		}

		if(node->QueryUnsignedAttribute("disk", &type.disk) != TIXML_SUCCESS) {
			fprintf(stderr, "Parser: bad member element: must have positive integer attribute 'disk'\n");
			break;
		} else if(type.disk == 0) {
			fprintf(stderr, "Parser: bad attribute: 'disk' must be a positive integer (was 0)\n");
			break;
		}

		// do this last so we don't have to free it if we broke out above
		const char *name = node->Attribute("name");
		if(name == nullptr) {
			fprintf(stderr, "Parser: bad member element: must have string attribute 'name'\n");
			break;
		}
		type.name = (char *) malloc(strlen(name) + 1);
		strcpy(type.name, name);

		server_types.push_back(type);
	}
	if(node != nullptr) { // failure, free and return null
		for(auto type : server_types) free(type.name);
		return nullptr;
	} // otherwise success

	system_config *config = (system_config *) malloc(sizeof(system_config));

	config->num_of_types = server_types.size();
	config->server_types = (server_type *) malloc(sizeof(server_type)*config->num_of_types);
	memcpy(config->server_types, server_types.data(), sizeof(server_type)*config->num_of_types);

	auto servers = std::vector<server_info>();
	for(auto i = 0; i < config->num_of_types; ++i) {
		server_type type = config->server_types[i];
		for(auto j = 0; j < type.limit; ++j) {
			servers.push_back(server_info{ &type, j, SS_OFFLINE, 0, type.cores, type.memory, type.disk });
		}
	}

	config->num_of_servers = servers.size();
	config->servers = (server_info *) malloc(sizeof(server_info)*config->num_of_servers);
	memcpy(config->servers, servers.data(), sizeof(server_info)*config->num_of_servers);

	return config;
}
