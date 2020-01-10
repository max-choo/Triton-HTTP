#include <stdio.h>
#include <stdlib.h>

void DieWithSystemMessage(const char *msg) {
	perror(msg);
	exit(1);
}
