#include <iostream>
#include <curl/curl.h>
#include <json-c/json.h>
#include <cstring>
#include<unistd.h>
#include "genie.h"


#define VERSION_STRING "5.0"


int main(int argc, char* argv[]) {
	json J;
	URL U;
	
	std::string text;
	const char *c_text;
	const char *json_object;
	if( argc > 1) {
		J.create_payload(argv[1]);
	} else {
		std::getline(std::cin, text);
		c_text = text.c_str();
		J.create_payload(c_text);
	}
	

	U.construct_full_url(API_URL, GEMINI_MODEL, API_KEY);

	Communicator C(&U, &J);
	C.send_command();
	json_object = C.get_response();
	//std::cout<<json_object<<std::endl;
	std::cout<<J.get_text(json_object)<<std::endl;
	//std::cout<<C.get_response() <<std::endl;
}
