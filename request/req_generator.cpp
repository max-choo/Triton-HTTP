#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

void req1();
void req2();
void req3();
void req4();
void wrong_req();
void wrong_req2();
void wrong_req3();
void wrong_req4();
void forbidden_req();

int main() {
	req1();
	return 0;
}

void req1() {

	ofstream outfile("request.txt");

	string message;

	message = "GET /index.html HTTP/1.1";
	message += '\r';
	message += '\n';

	message += "Host: localhost";
	message += '\r';
	message += '\n';

	message += "User-Agent: cse124-tester/1.0";
	message += '\r';
	message += '\n';

	message += "Connection: close";
	message += '\r';
	message += '\n';

	message += '\r';
	message += '\n';

	outfile << message;
	outfile.flush();

	outfile.close();



}
void req2() {

	ofstream outfile("request2.txt");

	string message;

	message = "GET /kiteen1.jpg HTTP/1.1";
	message += '\r';
	message += '\n';

	message += "Host: localhost";
	message += '\r';
	message += '\n';

	message += "User-Agent: cse124-tester/1.0";
	message += '\r';
	message += '\n';

	message += "Connection: close";
	message += '\r';
	message += '\n';

	message += '\r';
	message += '\n';

	outfile << message;
	outfile.flush();

	outfile.close();
}

void req3() {

	ofstream outfile("request3.txt");

	string message;

	message = "GET /kiteen1.jpg HTTP/1.1";
	message += '\r';
	message += '\n';

	message += "Host: localhost";
	message += '\r';
	message += '\n';

	message += "Modified: April 27th, 2018 02:11:48 GMT";
	message += '\r';
	message += '\n';

	message += '\r';
	message += '\n';

	outfile << message;
	outfile.flush();

	outfile.close();
}

void req4() {

	ofstream outfile("request4.txt");

	string message;

	message = "GET / HTTP/1.1";
	message += '\r';
	message += '\n';

	message += "Host: localhost";
	message += '\r';
	message += '\n';

	message += "User-Agent: cse124-tester/1.0";
	message += '\r';
	message += '\n';

	message += "Connection: close";
	message += '\r';
	message += '\n';

	message += '\r';
	message += '\n';

	outfile << message;
	outfile.flush();
}

void wrong_req() {

	ofstream outfile("wrong_request.txt");

	string message;

	message = "GET /kiteen1.jpg HTTP/1.1";
	message += '\r';
	message += '\n';

	message += "Host localhost";
	message += '\r';
	message += '\n';

	message += "Modified: April 27th, 2018 02:11:48 GMT";
	message += '\r';
	message += '\n';

	message += '\r';
	message += '\n';

	outfile << message;
	outfile.flush();

	outfile.close();
}

void wrong_req2() {

	ofstream outfile("wrong_request2.txt");

	string message;

	message = "GET /kiteen1.jpg HTTP/1.1";
	message += '\r';
	message += '\n';

	message += "User-Agent: cse124-tester/1.0";
	message += '\r';
	message += '\n';

	message += "Connection: close";
	message += '\r';
	message += '\n';

	message += "Modified: April 27th, 2018 02:11:48 GMT";
	message += '\r';
	message += '\n';

	message += '\r';
	message += '\n';

	outfile << message;
	outfile.flush();

	outfile.close();
}

void wrong_req3() {

	ofstream outfile("wrong_request3.txt");

	string message;

	message = "GET /index.html /kitten.jpg HTTP/1.1";
	message += '\r';
	message += '\n';

	message += "Host: localhost";
	message += '\r';
	message += '\n';

	message += "User-Agent: cse124-tester/1.0";
	message += '\r';
	message += '\n';

	message += "Connection: close";
	message += '\r';
	message += '\n';

	message += '\r';
	message += '\n';

	outfile << message;
	outfile.flush();
}

void wrong_req4() {

	ofstream outfile("wrong_request4.txt");

	string message;

	message = "GET ijghxfjdkhbzhv HTTP/1.1";
	message += '\r';
	message += '\n';

	message += "Host: localhost";
	message += '\r';
	message += '\n';

	message += "User-Agent: cse124-tester/1.0";
	message += '\r';
	message += '\n';

	message += "Connection: close";
	message += '\r';
	message += '\n';

	message += '\r';
	message += '\n';

	outfile << message;
	outfile.flush();

	outfile.close();

}
void forbidden_req() {

	ofstream outfile("forbidden.txt");

	string message;

	message = "GET /subdir/test.txt HTTP/1.1";
	message += '\r';
	message += '\n';

	message += "Host: localhost";
	message += '\r';
	message += '\n';

	message += "User-Agent: cse124-tester/1.0";
	message += '\r';
	message += '\n';

	message += "Connection: close";
	message += '\r';
	message += '\n';

	message += '\r';
	message += '\n';

	outfile << message;
	outfile.flush();
}
