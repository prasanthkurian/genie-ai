#include "genie.h"
#include <curl/curl.h>

static size_t callback(char *data, size_t size, size_t nmemb, void *clientp)
{
	size_t realsize = size * nmemb;
	struct memory *mem = (struct memory *)clientp;

	char *ptr =  (char *) realloc(mem->response, mem->size + realsize + 1);
	 
	if(ptr == nullptr)
		return 0;  // out of memory 
		 
	mem->response = ptr;
	memcpy(&(mem->response[mem->size]), data, realsize);
	mem->size += realsize;
	data[mem->size+1];
	mem->response[mem->size] = 0;

	return realsize;
}

const unsigned short URL::construct_full_url(const char* baseurl, char *model, char *api_key) {
		char* ret = 0;
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
const char* json::get_text(const char* response) {
	
	// Parse the JSON string into a JSON object
	struct json_object *json_obj = json_tokener_parse(response);

	// Check if parsing was successful
	if (json_obj == nullptr) {
		err(EXIT_FAILURE, "json object conversion failed");
	}

	// Access values from the JSON object
	struct json_object *candidates;
	struct json_object *content;
	struct json_object *parts;
	struct json_object *parts_content;
	struct json_object *text;
	struct json_object *text_content;

	json_object_object_get_ex(json_obj, "candidates", &candidates);
	content = json_object_array_get_idx(candidates, 0);
	
	json_object_object_get_ex(content, "content", &parts);
	
	json_object_object_get_ex(parts, "parts", &parts_content);
	text = json_object_array_get_idx(parts_content, 0);
	json_object_object_get_ex(text, "text", &text_content);
	
	const char* candidate_string = json_object_get_string(text_content);

	return candidate_string;
}


Communicator::Communicator(URL *U, json *J) {
	struct curl_slist *headers = nullptr;
	this->curl_object = curl_easy_init();
	if (this->curl_object == nullptr ) {
		err(EXIT_FAILURE, "curl initialization failed");
	}
	const char *url = U->get_full_url();
	const char* json_string = J->get_json_string();
	this->response ={0};
	headers = curl_slist_append(headers, "Content-Type: application/json");
	curl_global_init(CURL_GLOBAL_DEFAULT);
	
    	curl_easy_setopt(this->curl_object, CURLOPT_SSL_VERIFYPEER, 0L);
    	curl_easy_setopt(this->curl_object, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(this->curl_object, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(this->curl_object, CURLOPT_URL,url); 
    	curl_easy_setopt(this->curl_object, CURLOPT_CA_CACHE_TIMEOUT, 604800L);
	curl_easy_setopt(this->curl_object, CURLOPT_POSTFIELDSIZE, strlen(json_string));
	curl_easy_setopt(this->curl_object, CURLOPT_POSTFIELDS, json_string);
	
	curl_easy_setopt(this->curl_object, CURLOPT_WRITEFUNCTION, callback);
	curl_easy_setopt(this->curl_object, CURLOPT_WRITEDATA, (void *)&this->response);
	

}

void Communicator::send_command() {
	char *response_string;
	int response = curl_easy_perform(this->curl_object);
	/*
	if (check_response(response), response_string != 0) {
		err(EXIT_FAILURE, response_string); 
	}
	*/
	
}
char* Communicator::get_response() {
	char *buffer = (char*) this->response.response;
	return buffer;
}
Communicator::~Communicator() {
	curl_easy_cleanup(this->curl_object);
	curl_global_cleanup();
}

const char* json::get_json_string() {
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


