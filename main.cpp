#include <iostream>
#include <thread>
#include <cstring>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "httpd.h"
#define MAXPENDING 128
using namespace std;


/*
File Name: Main
Description: Handles and implements server logic
*/
void DieWithSystemMessage(const char *msg);
void HandleTCPClient(int clntSocket, string doc_root);
void HandleTCPClientThread(int clntSocket, string doc_root);

void usage(char * argv0)
{
	cerr << "Usage: " << argv0 << " listen_port docroot_dir" << endl;
}

int main(int argc, char *argv[])
{
	// Check for correct number of arguments
	if (argc != 3) {
		usage(argv[0]);
		return 1;
	}
	// Extract port number
	long int port = strtol(argv[1], NULL, 10);
	// Check if correct port number
	if (errno == EINVAL || errno == ERANGE) {
		usage(argv[0]);
		return 2;
	}
	// Check if port number is valid
	if (port <= 0 || port > USHRT_MAX) {
		cerr << "Invalid port: " << port << endl;
		return 3;
	}
	// Extract doc_root
	string doc_root = argv[2];
	
	int servSock;	// Declare server socket
	if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithSystemMessage("socket() failed");

	// Construct local address structure
	struct sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);

	// Bind to the local address
	if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
		DieWithSystemMessage("bind() failed");

	// Mark the socket so it will listen for incoming connection
	if (listen(servSock, MAXPENDING) < 0)
		DieWithSystemMessage("listen() failed");

	// Run forever
	for (;;) {
		// Declare client address
		struct sockaddr_in clntAddr;
		socklen_t clntAddrLen = sizeof(clntAddr);

		// Wait for a client request
		int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
		if (clntSock < 0)
			DieWithSystemMessage("accept() failed");

		/* clntSock is connected to a client */

		// spawn a thread for the client
		HandleTCPClient(clntSock, doc_root);

		start_httpd(port, doc_root);

	}

	return 0;
}
