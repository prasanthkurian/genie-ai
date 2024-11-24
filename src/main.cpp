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
	const char *json_object;
	J.create_payload("Text me");
	U.construct_full_url(API_URL, GEMINI_MODEL, API_KEY);
	std::cout<<J.display()<<std::endl;
	std::cout<<U.get_full_url()<<std::endl;

}
