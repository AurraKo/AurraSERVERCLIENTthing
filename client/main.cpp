#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <semaphore.h> 

//global variables
struct sockaddr_in addr;
pthread_t t, t_w;

int sd=-1;

//functions

//defines
#define ADDRESS "127.0.0.1"
#define PORT 8081

//threads
extern void *handler(void *arg);

int main(){
	int opt=1;
	sd = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	inet_pton(AF_INET, ADDRESS, &addr.sin_addr);
	pthread_create(&t, NULL, handler, NULL);
	
	for (;;);
}
