#include <iostream>
#include <string>
#include <deque>
/*
Class Name: Framer
Description:
	- HEADER FILE FOR FRAMER.CPP
	- Contains local variables for Framer and outlines class functionality
	- Frames input buffers into messages and appends to a request_buffer
	for later use.
*/
class Framer {
private:
	//local deque which contains the requests after framing
	std::deque<std::string> request_buffer;
	//local incomplete message
	std::string temp_message;
public:
	void append(uint8_t* buffer, int length);
	bool hasMessage() const;
	std::string topMessage() const;
	void popMessage();
	void print() const;
};
