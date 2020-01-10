#include <iostream>
#include <vector>
#include <string>

using namespace std;


/*
    Struct Name: RequestType
    Description: Encapsulates a Request from the client
*/
struct RequestType {
    bool clientError = false; //keeps state for 400 client error
	bool closeConnection = false; //whether to close connection or not
	bool endOfHeader = false; //state for end of file
    string method; //string for method
    string url; //relative file path
    string version; //http version
    vector<pair<string, string>> header; //vector of key-value pairs from header
	string fileType; //type of file (html, jpg, png)
	string optional; //optional body

};
