//#pragma once
#include <iostream>
#include <err.h>
#include <json-c/json.h>
#include <cstring>
#include <curl/curl.h>
#include<unistd.h>
#include <vector>
#include <fstream>

#define API_URL "https://generativelanguage.googleapis.com/v1beta/models"
#define GEMINI_MODEL std::getenv("GEMINI_MODEL")
#define API_KEY std::getenv("GOOGLE_API_KEY")
#define CONV_HIST_FILE std::getenv("CONV_HIST_FILE")
 


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
class book_keeper {
	private:
		std::string filename = CONV_HIST_FILE;
	        bool conversation_history_exists = true;	
	        bool conversation_history_regular = true;	
	        bool conversation_history_readable = true;
	        bool conversation_history_writable = true;
		std::vector<std::string> buffer;
	public:
		book_keeper();
		bool check_conversation_history();
		bool add_conversation_to_buffer(std::string yours, std::string genies);
		std::string get_string_from_vector();
		~book_keeper();
			
};

class display {
	private:
		void get_char_positions(std::string, std::string, std::vector<int>*);
	public:
		display();
		std::string handle_stream(std::string);
		std::string handle_bold(std::string, std::vector<int>);
		//std::string handle_italics();
		void print_dashs();
		~display();

};

extern void add_json_key_value(json_object*, const char*, const char*);

extern void add_json_object(json_object*, const char*, json_object*);
extern void add_to_json_array(json_object *, const char *, const char*);

