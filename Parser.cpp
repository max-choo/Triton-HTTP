#include <iostream>
#include <assert.h>
#include "Parser.hpp"
#include <string.h>
#include <sstream>
#include <vector>

/*
Class Name: Parser
Description: 
	Contains method implementation for requests by the user.
	Methods:
		resetRequest()
		hasRequested()
		hasHost()
		validateStartline()
		parseStartline()
		parseKeyValue()
		parse()
		printHeader()
*/

// Reset the request
void Parser::resetRequest()
{
	request.method = "";
	request.url = "";
	request.version = "";
	request.optional = "";
	request.header.clear();
	request.clientError = false;
	request.closeConnection = false;
	request.endOfHeader = false;

}

// Check if the first request has been made.
// Used to determine whether to parse startline or key-value
bool Parser::hasRequested(){
	return request.method != "" && request.url != "" && request.version != "";
}

//checks if there is a host key in the key value portion
void Parser::hasHost() {
	bool hasHost = false;
	string host = "Host";

	// Go through header key-value pairs and check if there's a host
	for (unsigned int i = 0; i < request.header.size(); i++) {
		pair<string, string> element_pair = request.header[i];
		if (strcmp(element_pair.first.c_str(), host.c_str()) == 0) hasHost = true;
	}

	// If there is no host, set error flag
	if (!hasHost) request.clientError = true;
}

// Validate startline format
void Parser::validateStartline() {

	if (request.url == "/") request.url = "/index.html";	// translate '/' into /index.html

	// Requested URL must start with '/'.
	if (request.url[0] != '/') request.clientError = true;

	// Ensure that we're using GET method
	const char* GETmethod = "GET";
	if (strcmp(request.method.c_str(), GETmethod) != 0) request.clientError = true;

	// Set fileType to html, jpg, or png depending on type of requested file.
	// If requested file is non of these types, set clientError flag
	if (request.url.find(".html") != string::npos) request.fileType = "html";
	else if (request.url.find(".jpg") != string::npos) request.fileType = "jpg";
	else if (request.url.find(".png") != string::npos) request.fileType = "png";
	else request.clientError = true;

	// Ensure that we're using HTTP/1.1
	const char* HTTPversion = "HTTP/1.1";
	if (strcmp(request.version.c_str(), HTTPversion) != 0) request.clientError = true;

}
// Parse startline
void Parser::parseStartline(std::string insstr) {

	stringstream ss(insstr);
	vector<string> http_request;
	string token;

	// breaking down line into string separated by space
	while(getline(ss, token, ' ')) {
		if (token.back() == '\r') token.pop_back();
		http_request.push_back(token);
	}

	// check if start line is correctly formated
	if (http_request.size() != 3) {
		request.clientError = true;
		return;
	}

	// Parsing start line
	request.method = http_request[0];
	request.url = http_request[1];
	request.version = http_request[2];

	validateStartline();

}

// Parse key-value block
void Parser::parseKeyValue(std::string insstr) {
	stringstream ss(insstr); //converts string to string stream
	pair<string, string> header_pair; //key-value pair
	string token; //token
	string value = ""; //initialize value
	bool isKey = true; //detects for key

	// breaking down line into string separated by space
	while(getline(ss, token, ':')) {
		// parsing key and store into first half of pair
		if (isKey) {
			// If colon is missing, set clientError flag
			if (strcmp(token.c_str(), insstr.c_str()) == 0) {
				request.clientError = true;
				return;
			}
			header_pair.first = token;
			isKey = false;
		}
		// parsing value. doesn't account for initial ' ' after ':'
		else {
			if(token.at(0) == ' ') {
				value += token.substr(1);
			} else {
				value += ':';
				value += token;
			}
		}
	}
	// Don't store '\r' at the end of string
	if (value.back() == '\r') value.pop_back();
	header_pair.second = value;
	request.header.push_back(header_pair);
	//handles closing the connection
	if (header_pair.first == "Connection" && header_pair.second == "close") request.closeConnection = true;
}

//parsers everything by calling subroutines
void Parser::parse(std::string insstr){
	// After end of header, add every optional messages
	if (request.endOfHeader){
		if (insstr != "") request.optional += insstr;
	}

	// At the end of header, check if header contains Host key
	// Set endOfHeader flag to true.
	else if (!insstr.empty() && insstr[0] == '\r') {
		hasHost();
		request.endOfHeader = true;
		return ;
	}
	// Parse header (startline and keyvalue)
	else {
		if(hasRequested()) parseKeyValue(insstr);
		else parseStartline(insstr);
	}
}

//prints header for debugging
void Parser::printHeader(){
	cout << "Method: " << request.method << "\tURL: " << request.url << "\tVerison: " << request.version << endl;
	for(unsigned int i = 0; i < request.header.size(); i++){
		pair<string, string> element_pair = request.header[i];
		cout<< "Key: " << element_pair.first << "\tValue: " << element_pair.second << endl;
    }
}

