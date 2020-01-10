#include <string>
#include <stdint.h>
#include <vector>
#include "Request.hpp"

using namespace std;

/*
Class Name: Parser
Description: 
	Header file for Parser.cpp. Contains local variables and 
	outlines the functionality for Parser.
	
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

class Parser {
public:
	RequestType request; //encapsulates the request
	void resetRequest();
	void parse(string insstr);
	void validateStartline();
	void parseStartline(std::string insstr);
	void parseKeyValue(std::string insstr);
	void printHeader();
	bool hasRequested();
	void hasHost();
};
