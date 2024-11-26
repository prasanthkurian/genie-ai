#include <iostream>
#include <json-c/json.h>
#include <cstring>
#include<unistd.h>
using namespace std;
// curl -k   -H "Content-Type: application/json"   -d "{\"contents\":[{\"parts\":[{\"text\":\"what is the C JSON parser in ubuntu\"}]}]}"   -X POST "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest:generateContent?key=$API_KEY"

const string gemini_url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest"; 
const string API_KEY="AIzaSyBsW7g1f2FcK04UHtjxudosUTka6rTxvhM";
const string full_url = gemini_url+":generateContent?key="+API_KEY;

struct memory {
	char *response;
	size_t size;
};
 
static size_t cb(char *data, size_t size, size_t nmemb, void *clientp)
{
	size_t realsize = size * nmemb;
	struct memory *mem = (struct memory *)clientp;

	char *ptr =  realloc(mem->response, mem->size + realsize + 1);
	 
	if(!ptr)
		//cout<<"out of memory"<<endl;
		return 0;  // out of memory 
		 
	mem->response = ptr;
	memcpy(&(mem->response[mem->size]), data, realsize);
	mem->size += realsize;
	data[mem->size+1];
	mem->response[mem->size] = 0;

	return realsize;
}


int main() {
	CURL *curl;
	CURLcode res;
	struct json_object *jsobj;
	struct curl_slist *headers=NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	const char* url = full_url.c_str();
	curl_global_init(CURL_GLOBAL_DEFAULT);
	string input;
	struct memory chunk = {0};
	char buf[10000];
	getline(cin,input);
	//const char *json_string ="{\"contents\":[{\"parts\":[{\"text\":\"How are you today ?\"}]}]}";
	strcat(buf,"{\"contents\":[{\"parts\":[{\"text\":\"");
	strcat(buf,input.c_str());
	strcat(buf,"\"}]}]}");
	const char *json_string = buf;
   	//const char * json_char_string = json_string.c_str();
	cout<<json_string<<endl;

	try {
		curl = curl_easy_init();
		if (!curl) {
			throw runtime_error("curl not intializaed");
		}
	} catch(runtime_error e) {
		cout << e.what() << endl;
		exit(1);
	}
    	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_URL,url); 
    	curl_easy_setopt(curl, CURLOPT_CA_CACHE_TIMEOUT, 604800L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(json_string));
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_string);
	
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
	//curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    	cout<<full_url<<endl;
	try{
		res = curl_easy_perform(curl);
		if( res != CURLE_OK ) {
			cout<<res<<endl;
			throw runtime_error("curl_easy_perform() failed");
		}
	} catch(runtime_error e) {
		cerr<<e.what()<<endl;
		exit(2);
	}
	cout<<chunk.response<<endl;
	free(chunk.response);

	curl_easy_cleanup(curl);
	curl_global_cleanup();
	cout<<"completed"<<endl;
	exit(0);
	
}


