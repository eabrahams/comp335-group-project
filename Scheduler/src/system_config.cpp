#include "system_config.h"
#include <type_traits>
static_assert(std::is_pod_v<server_type>, "server_type MUST be POD to be C-safe!");
static_assert(std::is_pod_v<server_info>, "server_info MUST be POD to be C-safe!");
static_assert(std::is_pod_v<system_config>, "system_config MUST be POD to be C-safe!");

#include <tinyxml.h>
#include <iostream>
#include <cstring>
#include <memory>
#include <vector>

inline namespace {
	/*
	check if an element has an attribute with a given name,
	allocating and copying the data to dest_ptr and returning true if it does,
	otherwise returning false and logging to stderr.
	*/
	bool copy_string_attribute(const TiXmlElement *node, const char *attr_name, char **dest_ptr) noexcept(true) {
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

	/*
	check if an element has a non-negative integer attribute with a given name,
	copying the value to dest_ptr and returning true if it does, otherwise
	returning false and logging to stderr.
	*/
	bool get_unsigned_int_attribute(const TiXmlElement *node, const char *attr_name, unsigned *dest_ptr) noexcept(true) {
		if(node->QueryUnsignedAttribute(attr_name, dest_ptr) != TIXML_SUCCESS) {
			std::cerr << "Parser: bad member element: must have unsigned integer attribute '" << attr_name << "'\n";
			return false;
		} else return true;
	}

	/*
	check if an element has a positive floating-point attribute with a given name,
	copying the value to dest_ptr and returning true if it does, otherwise
	returning false and logging to stderr.
	*/
	bool get_positive_float_attribute(const TiXmlElement *node, const char *attr_name, float *dest_ptr) noexcept(true) {
		if(node->QueryFloatAttribute(attr_name, dest_ptr) != TIXML_SUCCESS) {
			std::cerr << "Parser: bad member element: must have positive floating-point attribute '" << attr_name << "'\n";
		} else if(*dest_ptr == 0.0) {
			std::cerr << "Parser: bad attribute: '" << attr_name << "' must be a positive floating-point number (was 0)\n";
		} else return true;
		return false;
	}

	/*
	verify that the name of an element is a given value
	*/
	bool elem_name_is(const TiXmlElement *elem, const char *name) noexcept(true) {
		if(strcmp(name, elem->Value())) {
			std::cerr << "Parser: bad element type: expected '" << name << "', but got '" << elem->Value() << "'\n";
			return false;
		} else return true;
	}
}

system_config *parse_config(const char *path) noexcept(true) {
	TiXmlDocument doc;
	if(!doc.LoadFile(path)) return nullptr;
	TiXmlElement *root = doc.RootElement();
	if(!elem_name_is(root, "system")) return nullptr;
	TiXmlElement *nodes = root->FirstChildElement();
	if(!elem_name_is(nodes, "servers")) return nullptr;
	auto types = std::vector<server_type>(); // use a vector to avoid over-alloc or realloc
	TiXmlElement *node; // declare outside of loop to check if it finished properly
	for(node = nodes->FirstChildElement(); node != nullptr && elem_name_is(node, "server"); node = node->NextSiblingElement()) {
		server_type type;

		if(!get_unsigned_int_attribute(node, "limit", &type.limit)) break;
		if(!get_unsigned_int_attribute(node, "bootupTime", &type.bootTime)) break;
		if(!get_positive_float_attribute(node, "rate", &type.rate)) break;
		if(!get_unsigned_int_attribute(node, "coreCount", &type.max_resc.cores)) break;
		if(!get_unsigned_int_attribute(node, "memory", &type.max_resc.memory)) break;
		if(!get_unsigned_int_attribute(node, "disk", &type.max_resc.disk)) break;
		if(!copy_string_attribute(node, "type", &type.name)) break;

		types.push_back(type);
	}
	if(node != nullptr) { // failure, free and return null
		for(auto type : types) free(type.name);
		return nullptr;
	} // otherwise success

	system_config config;

	// have to make a copy of the server_type-s first, otherwise the vector will
	//.. still own the memory they live in when we initialize server_info-s with them
	config.num_types = types.size();
	config.types = static_cast<server_type *>(malloc(sizeof(server_type)*config.num_types));
	memcpy(const_cast<server_type *>(config.types), types.data(), sizeof(server_type)*config.num_types);

	// use a vector for this, again to avoid over-alloc or realloc
	auto servers = std::vector<server_info>();
	for(auto i = 0; i < config.num_types; ++i) {
		auto *type = config.types + i;
		for(auto j = 0; j < type->limit; ++j) {
			servers.push_back(server_info{ type, j, server_state::SS_INACTIVE, 0, type->max_resc });
		}
	}

	config.num_servers = servers.size();
	config.servers = static_cast<server_info *>(malloc(sizeof(server_info)*config.num_servers));
	memcpy(config.servers, servers.data(), sizeof(server_info)*config.num_servers);

	system_config *cfg_ptr = static_cast<system_config *>(malloc(sizeof(system_config)));
	memcpy(cfg_ptr, &config, sizeof(system_config));

	return cfg_ptr;
}

void free_config(system_config *config) noexcept(true) {
	for(auto i = 0; i < config->num_types; ++i) free(config->types[i].name);
	free(const_cast<server_type *>(config->types));
	free(config->servers);
	free(config);
}

const server_type *type_by_name(const system_config *config, const char* name) noexcept(true) {
	for(auto i = 0; i < config->num_types; ++i) {
		if(!strcmp(config->types[i].name, name)) return &config->types[i];
	}
	return nullptr;
};


server_info *servers_by_type(const system_config *config, const server_type *type) noexcept(true) {
	for(auto i = 0; i < config->num_servers; ++i) {
		if(config->servers[i].type == type) return &config->servers[i];
	}
	return nullptr;
}

bool server_info::update(const server_state &state, const unsigned &time, const resource_info &resc) noexcept(true) {
	if(resc <= type->max_resc) {
		this->state = state;
		this->avail_resc = resc;
		return true;
	} else return false;
}

bool update_server(server_info *server, server_state state, unsigned time, resource_info resc) noexcept(true) {
	return server->update(state, time, resc);
}

void server_info::reset() noexcept(true) {
	avail_resc = type->max_resc;
}

void reset_server(server_info *server) noexcept(true) {
	//TODO: figure out what avail_time is, currently assuming it's "time until available"
	server->reset();
}