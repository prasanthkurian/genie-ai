//#pragma once
#include <iostream>
#include <err.h>
#include <json-c/json.h>
#include <cstring>
#include <curl/curl.h>
#include<unistd.h>

#define API_URL "https://generativelanguage.googleapis.com/v1beta/models"
#define GEMINI_MODEL "gemini-1.5-flash-001"
#define API_KEY "AIzaSyBsW7g1f2FcK04UHtjxudosUTka6rTxvhM"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

struct memory {
	char *response;
	size_t size;
};

class URL {
	private:
		char *base_url = nullptr;
		char *model = nullptr;
		char *api_key = nullptr;
		char *full_url = nullptr;
	public:
		const char* get_full_url();
		const unsigned short construct_full_url(const char* baseurl, char* model, char* api_key);

};

class json {
	private:
		json_object *js;
	public:
		void create_payload(const char* text);
		const char* get_json_string();
		const char* get_text(const char* response);
};

class Communicator {
	private:
		CURL *curl_object;
		CURLcode res;
		struct memory response;
	public:
		Communicator(URL *U, json *J);
		void send_command();
		char* get_response();
		~Communicator();

};

extern void add_json_key_value(json_object*, const char*, const char*);

extern void add_json_object(json_object*, const char*, json_object*);
extern void add_to_json_array(json_object *, const char *, const char*);

