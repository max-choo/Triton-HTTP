#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct ResponseType {
	bool clientError = false;	// keep state of 400 client error
	bool fileFound = true;	// whether file is found
	bool permission = true;	// whether the file has permission
	size_t size;	// length of the file in size_t format
	string last_modified;	// string for last modified time
	string content_type;	// string for content type
	string content_length;	// string for content length
	string message;	// response header message
};
