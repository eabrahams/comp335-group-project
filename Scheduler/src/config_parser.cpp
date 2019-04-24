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
	if(strcmp("system", root->Value())) {
		fprintf(stderr, "Parser: bad root element: expected 'system', but got '%s'\n", root->Value());
		return nullptr;
	}
	TiXmlElement *nodes = root->FirstChildElement();
	if(strcmp("servers", nodes->Value())) {
		fprintf(stderr, "Parser: bad container element: expected 'servers', but got '%s'\n", nodes->Value());
		return nullptr;
	}
	auto servers = std::vector<server_type>();
	TiXmlElement *node;
	for(node = nodes->FirstChildElement(); node != nullptr; node = node->NextSiblingElement()) {
		server_type server;

		if(strcmp("server", node->Value())) {
			fprintf(stderr, "Parser: bad member element: expected 'server', but got '%s'\n", node->Value());
			break;
		}
		
		if(node->QueryIntAttribute("limit", &server.limit) != TIXML_SUCCESS) {
			fprintf(stderr, "Parser: bad member element: must have positive integer attribute 'limit'\n");
			break;
		} else if(server.limit <= 0) {
			fprintf(stderr, "Parser: bad attribute: 'limit' must be a positive integer (was %i)\n", server.limit);
			break;
		}

		if(node->QueryIntAttribute("bootupTime", &server.bootTime) != TIXML_SUCCESS) {
			fprintf(stderr, "Parser: bad member element: must have non-negative integer attribute 'bootupTime'\n");
			break;
		} else if(server.bootTime < 0) {
			fprintf(stderr, "Parser: bad attribute: 'bootupTime' must be a non-negative integer (was %i)\n", server.bootTime);
			break;
		}

		if(node->QueryFloatAttribute("rate", &server.rate) != TIXML_SUCCESS) {
			fprintf(stderr, "Parser: bad member element: must have positive floating-point attribute 'rate'\n");
			break;
		} else if(server.rate < 0) {
			fprintf(stderr, "Parser: bad attribute: 'rate' must be a positive floating-point number (was %f)\n", server.rate);
			break;
		}

		if(node->QueryIntAttribute("coreCount", &server.cores) != TIXML_SUCCESS) {
			fprintf(stderr, "Parser: bad member element: must have positive integer attribute 'coreCount'\n");
			break;
		} else if(server.cores <= 0) {
			fprintf(stderr, "Parser: bad attribute: 'coreCount' must be a positive integer (was %i)\n", server.cores);
			break;
		}

		if(node->QueryIntAttribute("memory", &server.memory) != TIXML_SUCCESS) {
			fprintf(stderr, "Parser: bad member element: must have positive integer attribute 'memory'\n");
			break;
		} else if(server.memory <= 0) {
			fprintf(stderr, "Parser: bad attribute: 'memory' must be a positive integer (was %i)\n", server.memory);
			break;
		}

		if(node->QueryIntAttribute("disk", &server.disk) != TIXML_SUCCESS) {
			fprintf(stderr, "Parser: bad member element: must have positive integer attribute 'disk'\n");
			break;
		} else if(server.disk <= 0) {
			fprintf(stderr, "Parser: bad attribute: 'disk' must be a positive integer (was %i)\n", server.disk);
			break;
		}

		// do this last so we don't have to free it if we broke out above
		const char *name = node->Attribute("name");
		if(name == nullptr) {
			fprintf(stderr, "Parser: bad member element: must have string attribute 'name'\n");
			break;
		}
		server.name = (char *) malloc(strlen(name) + 1);
		strcpy(server.name, name);

		servers.push_back(server);
	}
	if(node == nullptr) { // success
		system_config *config = (system_config *) malloc(sizeof(system_config));
		config->num_of_types = servers.size();
		config->server_types = (server_type *) malloc(sizeof(server_type)*config->num_of_types);
		memcpy(config->server_types, servers.data(), sizeof(server_type)*config->num_of_types);
		return config;
	} else { // failure, free and return null
		for(auto server : servers) free(server.name);
		return nullptr;
	}
}
