#include <iostream>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <sys/types.h>
#include <sys/stat.h>
#include "Framer.hpp"
#include "Parser.hpp"
#include <stdlib.h>
#include "Response.hpp"
#include <time.h>
#include <fcntl.h>
#include <sys/sendfile.h>

/*
File Name: ResponseHelper.hpp
Description: 
	Contains helper functions to generate response.
	Methods:
		doesEscapeDocRoot()
		mapURL()
		checkPermission()
		setType()
		generateMessage()
		generateResponse()

*/

//given some path, checks if it escapes document root
bool doesEscapeDocRoot(char* path, const char* doc_root){
	string path_string(path);
	string doc_string(doc_root);
	if(path_string.find(doc_string) == string::npos){
		return true;
	}
	return false;

}

//maps the rel path and doc root to absolute path
//returns true if successful
bool mapURL(const char* rel_path, const char* doc_root, char *& abs_path) {
	char buf[PATH_MAX+1];
	string temp_rel(rel_path);
 	string temp_root(doc_root);
 	string path = temp_root + temp_rel;
	//this will give the full path to the specified rel_path
	char * full_path = realpath(path.c_str(), buf); 
	//check if the full_path exists
	if(!full_path) {
    	return false;
  	}
	//if the full path escapes doc root, return false
	else if(doesEscapeDocRoot(full_path, doc_root)) return false;
	//construct the file path
	else{
		//construct the file path
    	abs_path = full_path;
		return true;
	}
}

// Check permission for the file
bool checkPermission(const char * pathname) {
	struct stat statbuf;
	if (stat(pathname, &statbuf) < 0) return false;
	else {
		mode_t mode = statbuf.st_mode; 
		// Check if the file is world-readable
		if (mode%8 < 4) return false;
		else return true;
	}
}

// Set the file type
void setType(RequestType request, ResponseType &response) {

	const char* texthtml = "html";
	const char* imagejpg = "jpg";
	const char* imagepng = "png";

	if (strcmp(request.fileType.c_str(), texthtml) == 0) response.content_type = "text/html";
	else if (strcmp(request.fileType.c_str(), imagejpg) == 0) response.content_type = "image/jpeg";
	else if (strcmp(request.fileType.c_str(), imagepng) == 0) response.content_type = "image/png";

}

//takes response and code and returns a string representation of the message
string generateMessage(ResponseType response, string code) {
	string message;
	message = "HTTP/1.1 ";
	message += code;
	message += '\r';
	message += '\n';

	message += "Server: Apache";
	message += '\r';
	message += '\n';

	if (response.last_modified != "") {
		message += "Last-Modified: ";
		message += response.last_modified;
		message += '\r';
		message += '\n';
	}
	if (response.content_type != "") {
		message += "Content-Type: ";
		message += response.content_type;
		message += '\r';
		message += '\n';
	}
	
	if (response.content_length != "") {
		message += "Content-Length: ";
		message += response.content_length;
		message += '\r';
		message += '\n';
	}

	message += '\r';
	message += '\n';

	return message;
}

//generates the response based on the request and file path
void generateResponse(RequestType request, ResponseType &response, const char* pathname) {
	string code;
	if (response.clientError) code = "400 Client Error";
	else if (!response.fileFound) code = "404 Not Found";
	else if (!response.permission) code = "403 Forbidden";
	else {
		code = "200 OK";
		// set content_type
		setType(request, response);
		// set content_length
		struct stat statbuf;
		stat(pathname, &statbuf);
		off_t st_size = statbuf.st_size;
		response.content_length = to_string(st_size);
		response.size = st_size;
		// Set last_modified
		char last_modified[100];
		time_t rawtime;
		struct tm * timeinfo;
		time (&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(last_modified, 100, "%c GMT", timeinfo);
		response.last_modified.assign((const char*)last_modified);
	}
	response.message = generateMessage(response, code);
}
