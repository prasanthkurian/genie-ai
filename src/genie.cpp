#include "genie.h"

const unsigned short URL::construct_full_url(const char* baseurl, char *model, char *api_key) {
		char* ret =0;
		unsigned short int full_url_length = 0;
		char *iterator = nullptr;
		this->base_url = (char* ) malloc(strlen(baseurl)+1);
		if(this->base_url == nullptr) {
			err(EXIT_FAILURE, "malloc");
			return EXIT_FAILURE;
		}
		ret = strcpy(this->base_url, baseurl);
		if(ret == nullptr) {
			err(EXIT_FAILURE, "strcpy");
			return EXIT_FAILURE;
		}
		
		this->model = (char* ) malloc(strlen(model)+1);
		if(this->model == NULL) {
			err(EXIT_FAILURE, "malloc");
			return EXIT_FAILURE;
		}
		ret = strcpy(this->model, model);
		if(ret == NULL) {
			err(EXIT_FAILURE, "strcpy");
			return EXIT_FAILURE;
		}
		
		
		this->api_key = (char* ) malloc(strlen(api_key)+1);
		if(this->api_key == NULL) {
			err(EXIT_FAILURE, "malloc");
			return EXIT_FAILURE;
		}
		ret = strcpy(this->api_key, api_key);
		if(ret == NULL) {
			err(EXIT_FAILURE, "strcpy");
			return EXIT_FAILURE;
		}

			
		full_url_length = strlen(base_url)+strlen(model)+strlen(":generateContent?key=")+strlen(api_key)+4;
		this->full_url = (char* )malloc(full_url_length);
		if(this->full_url == nullptr) {
			err(EXIT_FAILURE, "malloc");
			return EXIT_FAILURE;
		}
		iterator = this->full_url;
		ret = strcpy(iterator,this->base_url);
		if(ret == NULL) {
			err(EXIT_FAILURE, "strcpy");
			return EXIT_FAILURE;
		}
		iterator = iterator + strlen(this->base_url);
		*iterator = '/';
		iterator++;	
		ret = strcpy(iterator,this->model);
		if(ret == nullptr) {
			err(EXIT_FAILURE, "strcpy");
			return EXIT_FAILURE;
		}
		
		iterator = iterator + strlen(this->model);
		ret = strcpy(iterator,":generateContent?key=");
		if(ret == nullptr) {
			err(EXIT_FAILURE, "strcpy");
			return EXIT_FAILURE;
		}
		iterator = iterator + strlen(":generateContent?key=");
		ret = strcpy(iterator,this->api_key);
		if(ret == NULL) {
			err(EXIT_FAILURE, "strcpy");
			return EXIT_FAILURE;
		}
			
		return EXIT_SUCCESS;

}
const char* URL::get_full_url() {
	return full_url;
}

void json::create_payload(const char *text) {

	static json_object *t = json_object_new_array();
	add_to_json_array(t, "text", text);
			
	static json_object *parts = json_object_new_object();
	add_json_object(parts, "parts", t);

	static json_object *content = json_object_new_array();
	json_object_array_add(content, parts);

	static json_object *c = json_object_new_object();
	add_json_object(c, "contents", content);
	this->js = c;
}

const char* json::display() {
	return json_object_to_json_string(this->js);
}
void add_json_key_value(json_object* js, const char *key, const char* value) {
	json_object_object_add(js, key, json_object_new_string(value));
}

void add_json_object(json_object* js, const char *key, json_object *part) {
	json_object_object_add(js, key, part);
}

void add_to_json_array(json_object *jarray, const char *key, const char* val) {
	json_object *j = json_object_new_object();
	add_json_key_value(j, key, val);
	json_object_array_add(jarray, j);
}


