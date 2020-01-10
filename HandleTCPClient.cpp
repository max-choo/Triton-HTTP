#include "ResponseHelper.hpp"
/*
File Name: HandleTCPClient
Description: 
	Handles Client Requests and generates responses.
	Methods:
		HandleTCPClientThread()
		HandleTCPClient()
*/

// Handle client request by receving message from client, framing and parsing, generating the response, and sending back to client.
void HandleTCPClientThread(int clntSocket, string doc_root) {
	printf("handling client %d\n", clntSocket);

	// set timeout so that when client doesn't send message for 5 seconds, close the connection
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	setsockopt (clntSocket, SOL_SOCKET, SO_RCVTIMEO,(char*) &timeout,sizeof(timeout));
	setsockopt (clntSocket, SOL_SOCKET, SO_SNDTIMEO,(char*) &timeout,sizeof(timeout));

	Framer framer;
	Parser parser;

	uint8_t buffer[512];
	memset(buffer, 0 ,512);

	int n;	// indicates the number of bytes received from client

	// receive message from client
	n = recv(clntSocket,buffer, 512, 0);

	if (n < 0 && errno == EWOULDBLOCK) {
		printf("closing socket due to timeout\n");
		close(clntSocket);
		return;
	}


	/* This loop receives the string until the end of stream */
	while(n > 0) {
		/* Framing */
		framer.append(buffer, n);

		/* Parsing */
		string thisMessage;
		while (framer.hasMessage()) {
			thisMessage = framer.topMessage();
			framer.popMessage();
			parser.parse(thisMessage);

			/* Handle Response */
			if (parser.request.endOfHeader) {
				RequestType request = parser.request;

				parser.resetRequest();
				ResponseType response;

				if (request.clientError) response.clientError = true;
				char* abs_path;

				if (!response.clientError) {
					response.fileFound = mapURL(request.url.c_str(), doc_root.c_str(), abs_path);
					response.permission = checkPermission(abs_path);
				}

				// Generate response based on request and file path
				generateResponse(request, response, abs_path);
				// Send response header message back to client
				send(clntSocket, response.message.c_str(), response.message.size(), 0);

				// Send response body to client
				int in_fd = open(abs_path,0);
				sendfile(clntSocket, in_fd, 0, response.size);

				// If client requested closeConnection, close the connection.
				if (request.closeConnection) {
					close(clntSocket);
					return;
				}

			}
		}

		/* check if there's more to receive */
		memset(buffer, 0 ,512);
		n = recv(clntSocket, buffer, 512, 0);

		if (n < 0 && errno == EWOULDBLOCK) {
			printf("closing socket due to timeout\n");
			close(clntSocket);
			return;
		}


	}

}

// Spawn a thread per each client's request
void HandleTCPClient(int clntSocket, string doc_root) {
	thread threadObj(HandleTCPClientThread, clntSocket, doc_root);
	threadObj.join();
}
