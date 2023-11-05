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
struct sockaddr_storage addr_s;
int addr_sz;
pthread_t t[10], t_r[10];
sem_t sem;

int sd=-1, ns=-1;

//functions


//threads
extern void *handler(void *arg);

int main(){
	sem_init(&sem, 0, 1);
	int opt=1;
	sd = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(8081);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

	bind(sd, (struct sockaddr*)&addr, sizeof(addr));
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));  

	listen(sd, 10); // 10 clients
	
	//accept the the handle inside the handle thread
	//basically start 10 handle threads.
	//each starting a reading thread
	//and interally checking the connection status
	//if its borked then reconnect.
	//but it will have 10 constant connection handles.
	//no more infinite threads.

   int v[10]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};	
	for (int i=0; i<10; i++){
		pthread_create(&t[i], NULL, handler, (void*)&v[i]);
		sleep(1);
	}
	
	for (;;);
	sem_destroy(&sem);
}
