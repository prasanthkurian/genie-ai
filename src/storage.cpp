#include "genie.h"
//#include <fstream>
#include <filesystem>
//#include <vector>
/* Keeper is a simple Class which can read content from file and store it in a vector 
   it can add an entry to the vector
   It will update it back to file

*/

book_keeper::book_keeper() {
	std::string line;
	std::ifstream file;
	filename = CONV_HIST_FILE;
	if(std::filesystem::exists(filename) == false ) {
		conversation_history_exists = false;
		return;
	}
	if (std::filesystem::is_regular_file(filename) == false ) {
		conversation_history_readable = false;
		return;
	}

	file.open(filename);
	if (file.is_open() == false ){
		conversation_history_readable = false;
		return;
	}
	while(! file.eof()) {
		getline(file,line);
		buffer.push_back(line);
	}
	
	file.close();
				
}

bool book_keeper::check_conversation_history() {
	bool truth = true;
	if (conversation_history_exists == false ) {
		std::cout<<"No Conversation history. Check the existance of "<<filename<<std::endl;
		truth = false;
	}
	if (conversation_history_regular == false ) {
		std::cout<<filename<<" not a regular file. Check the file! "<<std::endl;
		truth = false;
	}
	if (conversation_history_readable == false ) {
		std::cout<<filename<<" not readable. Check the file! "<<std::endl;
		truth = false;
	}
	if (conversation_history_writable == false ) {
		std::cout<<filename<<" not writable. Check the file! "<<std::endl;
		truth = false;
	}
	if(buffer.size() == 0 ) {
		std::cout<<"Error in reading from file "<<filename<<std::endl;
		truth = false;
	}
	return truth;
}
bool book_keeper::add_conversation_to_buffer(std::string yours, std::string genies) {
	char time[100];
	std::ofstream writerfile;		
	writerfile.open(filename, std::ios::app);
	if(writerfile.is_open() == false ) {
		conversation_history_writable = false;
		std::cout<<"cannot open file for writing"<<std::endl;
	}
	std::string input_string;
	std::time_t now = std::time(nullptr);
	std::strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
	input_string = std::string(time)+": "+"Q :"+yours;
	this->buffer.push_back(input_string);
	if (conversation_history_writable == true ) {
		writerfile<<input_string<<std::endl;
	}
			
	now++;
	std::strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
	input_string = std::string(time)+": "+"A :"+genies;
	this->buffer.push_back(input_string);
	if (conversation_history_writable == true ) {
		writerfile<<input_string<<std::endl;
	}
	writerfile.close();
	return true;
}
		


std::string book_keeper::get_string_from_vector() {
	std::vector<std::string>::iterator it;
	std::string conversation = "";
	for(it = this->buffer.begin(); it != this->buffer.end(); it++ ) {
		conversation = conversation + "\n" +*it;
	}
	return conversation;
}
			      		
book_keeper::~book_keeper() {
}




