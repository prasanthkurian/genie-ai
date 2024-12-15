#include <sstream>
#include <sys/ioctl.h>
#include <termio.h>
#include "genie.h"


display::display() {
	
}

std::string display::handle_stream(std::string  text) {
	std::string final_text;
	std::stringstream s(text);
	std::stringstream f;
	std::vector<int> stars;
	std::vector<int> whites;
	std::string line;
	while (std::getline(s, line)) {
		get_char_positions(line, "*", &stars);
		if(stars.size() != 0 ) { 
			line = handle_bold(line,stars);
		}
		f<<line<<std::endl;
		stars.clear();
	
	}
	return f.str();
}

void display::get_char_positions(std::string line, std::string character, std::vector<int>* list) {
	int position = 0;
	for(position = line.find(character, position); position != std::string::npos || position < line.length(); position = line.find(character, position+1) ) {
		list->push_back(position);
	}
}


void display::print_dashs() {
	int i = 0;
	struct winsize win;
	ioctl(0, TIOCGWINSZ, &win);
	for(i= 0; i< win.ws_col; i++) {
		std::cout<<"-";
	}
	std::cout<<std::endl;
}

std::string display::handle_bold(std::string line, std::vector<int> stars) {
	std::vector<int>::iterator it = stars.begin();
	std::vector<int> char_positions_to_be_erased;
	for(; it + 3 < stars.end(); it++) {
		int first = *it, second = *(it+1), rear_first =*(it+2), rear_last = *(it+3); 
		if(second == first+1) {
			if(rear_first != second+1) {
				if(rear_last == rear_first+1) {
					char_positions_to_be_erased.insert(char_positions_to_be_erased.end(), {first, second, rear_first, rear_last});
					it = it + 3;
				}
			}
		}
	}
	int erase_count = 0; 
	for(std::vector<int>::iterator in = char_positions_to_be_erased.begin(); in < char_positions_to_be_erased.end(); in++) {
		line.erase(*in-erase_count, 1);
		erase_count++;

	}

	return line;
}

	
	
/*
std::string process_line(std::string line) {
	std::cout<<std::count(line
}	

std::string display::handle_bold() {
}
std::string display::handle_italics() {
}
std::string display::handle_bullets() {
}
std::string display::hand_table() {
	}
std::string display::handle_code_block() {
	}
	*/
display::~display() {
}
