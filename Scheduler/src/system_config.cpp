#include <tinyxml.h>
#include <iostream>
#include <cstring>
#include <memory>
#include <vector>
#include <numeric>

#include "system_config.h"

bool copy_string_attribute(const TiXmlElement *const node, const char *const attr_name, char **const dest_ptr) noexcept(true) {
	const char *attr_ptr = node->Attribute(attr_name);
	if(attr_ptr == nullptr) {
		std::cerr << "Parser: bad member element: must have string attribute '" << attr_name << "'\n";
		return false;
	} else {
		*dest_ptr = static_cast<char *>(malloc(strlen(attr_ptr) + 1));
		strcpy(*dest_ptr, attr_ptr);
		return true;
	}
}

bool get_unsigned_int_attribute(const TiXmlElement *const node, const char *const attr_name, unsigned *const dest_ptr) noexcept(true) {
	if(node->QueryUnsignedAttribute(attr_name, dest_ptr) != TIXML_SUCCESS) {
		std::cerr << "Parser: bad member element: must have non-negative integer attribute '" << attr_name << "'\n";
		return false;
	} else return true;
}

bool get_positive_int_attribute(const TiXmlElement *const node, const char *const attr_name, unsigned *const dest_ptr) noexcept(true) {
	if(node->QueryUnsignedAttribute(attr_name, dest_ptr) != TIXML_SUCCESS) {
		std::cerr << "Parser: bad member element: must have positive integer attribute '" << attr_name << "'\n";
	} else if(*dest_ptr == 0) {
		std::cerr << "Parser: bad attribute: '" << attr_name << "' must be a positive integer (was 0)\n";
	} else return true;
	return false;
}

bool get_positive_float_attribute(const TiXmlElement *const node, const char *const attr_name, float *const dest_ptr) noexcept(true) {
	if(node->QueryFloatAttribute(attr_name, dest_ptr) != TIXML_SUCCESS) {
		std::cerr << "Parser: bad member element: must have positive floating-point attribute '" << attr_name << "'\n";
	} else if(*dest_ptr == 0.0) {
		std::cerr << "Parser: bad attribute: '" << attr_name << "' must be a positive floating-point number (was 0)\n";
	} else return true;
	return false;
}

bool elem_name_is(const TiXmlElement *const elem, const char *const name) noexcept(true) {
	if(strcmp(name, elem->Value())) {
		std::cerr << "Parser: bad element type: expected '" << name << "', but got '" << elem->Value() << "'\n";
		return false;
	} else return true;
}


system_config *parse_config(const char *path) noexcept(true) {
	TiXmlDocument doc;
	if(!doc.LoadFile(path)) return nullptr;
	TiXmlElement *root = doc.RootElement();
	if(!elem_name_is(root, "system")) return nullptr;
	TiXmlElement *nodes = root->FirstChildElement();
	if(!elem_name_is(nodes, "servers")) return nullptr;
	auto server_types = std::vector<server_type>(); // use a vector to avoid over-alloc or realloc
	TiXmlElement *node; // declare outside of loop to check if it finished properly
	for(node = nodes->FirstChildElement(); node != nullptr && elem_name_is(node, "server"); node = node->NextSiblingElement()) {
		server_type type;

		if(!get_positive_int_attribute(node, "limit", &type.limit)) break;
		if(!get_unsigned_int_attribute(node, "bootupTime", &type.bootTime)) break;
		if(!get_positive_float_attribute(node, "rate", &type.rate)) break;
		if(!get_positive_int_attribute(node, "coreCount", &type.cores)) break;
		if(!get_positive_int_attribute(node, "memory", &type.memory)) break;
		if(!get_positive_int_attribute(node, "disk", &type.disk)) break;
		if(!copy_string_attribute(node, "type", &type.name)) break;

		server_types.push_back(type);
	}
	if(node != nullptr) { // failure, free and return null
		for(auto type : server_types) free(type.name);
		return nullptr;
	} // otherwise success

	//system_config *config = (system_config *) malloc(sizeof(system_config));

	size_t cfg_num_types = server_types.size();
	server_type * cfg_types = (server_type *) malloc(sizeof(server_type)*cfg_num_types);
	memcpy(cfg_types, server_types.data(), sizeof(server_type)*cfg_num_types);

	auto servers = std::vector<server_info>();
	for(auto i = 0; i < cfg_num_types; ++i) {
		server_type type = cfg_types[i];
		for(auto j = 0; j < type.limit; ++j) {
			servers.push_back(server_info{ &type, j, SS_OFFLINE, 0, type.cores, type.memory, type.disk });
		}
	}

	size_t cfg_num_servers = servers.size();
	server_info *cfg_servers = (server_info *) malloc(sizeof(server_info)*cfg_num_servers);
	memcpy(cfg_servers, servers.data(), sizeof(server_info)*cfg_num_servers);

	system_config config{ cfg_types, cfg_num_types, cfg_servers, cfg_num_servers };
	system_config *cfg_ptr = (system_config *) malloc(sizeof(system_config));
	memcpy(cfg_ptr, &config, sizeof(system_config));

	return cfg_ptr;
}

void release_config(system_config *const config) noexcept(true) {
	for(auto i = 0; i < config->num_types; ++i) free(config->types[i].name);
	free(const_cast<server_type *>(config->types));
	free(config->servers);
	free(config);
}

server_info *server_of_type(const system_config *const config, const server_type *const type) noexcept(true) {
	for(auto i = 0; i < config->num_servers; ++i) {
		if(config->servers[i].type == type) return &config->servers[i];
	}
	return nullptr;
}

void reset_server(server_info *const server) noexcept(true) {
	//TODO: figure out what avail_time is
	server->avail_cores = server->type->cores;
	server->avail_memory = server->type->memory;
	server->avail_disk = server->type->disk;
}