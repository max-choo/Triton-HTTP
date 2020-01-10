/*
Class Name: Framer
Description:
	- Contains implementation for Framer methods
	- Frames input buffers into messages and appends to a request_buffer
	for later use.
*/
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Framer.hpp"

using namespace std;

//takes in a buffer and constructs the message for given length
//appends message to to deque
//no return value
void Framer::append(uint8_t* buffer, int length)
{
	int i = 0; //iterator
	string message; //builds one message

	// If there's temp message, initialize message with temp message.
	if (temp_message.size() > 0) {
		message = temp_message;
		this->temp_message = "";
	}

	//build the message
	while (i < length) {
		//check for CRLF
		if (buffer[i] == '\r') {
			message += '\r';
			this->request_buffer.push_back(message);
			message = "";
		} else {
			//append only if not newline
			if (buffer[i] != '\n') message += buffer[i];
		}
		i++;
	}

	// Store incomplete message into temp message
	if (message.size() > 0) {
		this->temp_message = message;
	}
}

//checks if a the request buffer contains a message
//returns true or false
bool Framer::hasMessage() const
{
	if (!this->request_buffer.empty()) return true;
	else return false;
}

//returns the top message in the Framer as a string
string Framer::topMessage() const
{
	if(!this->request_buffer.empty())
		return this->request_buffer.front();
	else return "";
	
}

//pops the front message in the framer
//no return value
void Framer::popMessage()
{
	if(!this->request_buffer.empty())
		this->request_buffer.pop_front();
	else return;
}

//prints the elements in the framer
void Framer::print() const
{
	if (hasMessage()) printf("top message is %s\n", topMessage().c_str());
	for (unsigned int i=0; i < request_buffer.size(); i++) {
		printf("printing: %s\n", request_buffer[i].c_str());
	}

}
