#include <iostream>
#include <curl/curl.h>
#include <json-c/json.h>
#include <cstring>
#include<unistd.h>
#include <limits>
#include "genie.h"


#define VERSION_STRING "5.0"

int get_input(std::string *full_text) {
	std::string text ="";
	std::string newline ="";
	while(std::getline(std::cin, text)) {
		if ( text == ".." ) {
			return 1;
		}
		*full_text = *full_text + newline + text;
		newline ="\n";
	}
	return 0;
}

int main(int argc, char* argv[]) {
	json J;
	URL U;
	book_keeper B;

	
	std::string full_text ="";
	std::string message = "";
	std::string past_conversation;
	std::string my_question;
	std::string genies_reply;
	const char *c_text;
	const char *json_object;
	unsigned short string_length = 0;
	if( argc > 1 ) {
		message = argv[1];
	}
       
	if(!std::cin.eof()){
		if (get_input(&my_question) == 0 ) {
		       my_question = my_question + "\n";	

		}
	}
	my_question = my_question+message;
	if(B.check_conversation_history() == true ) {
		past_conversation = B.get_string_from_vector();
		full_text = past_conversation+"Q: "+my_question;
	} else {
		full_text = my_question;
	}
	c_text = full_text.c_str();
	J.create_payload(c_text);
	U.construct_full_url(API_URL, GEMINI_MODEL, API_KEY);
	Communicator C(&U, &J);
	C.send_command();
	json_object = C.get_response();
	genies_reply = J.get_text(json_object);
	std::cout<<"-------------------------------------------------------------------------"<<std::endl;
	std::cout<<genies_reply<<std::endl;
	std::cout<<"-------------------------------------------------------------------------"<<std::endl;
	B.add_conversation_to_buffer(my_question,genies_reply);
}
