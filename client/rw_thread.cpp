#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <iostream>
#include <semaphore.h>
#include <errno.h>
using namespace std;

//global variables
extern pthread_t t, t_w;
extern int sd;

// thread functions
void *write_thread(void *arg);

// structs
extern struct sockaddr_in addr;


void *handler(void *arg){
	int addr_sz, ns;

	//connect the the server
	addr_sz = sizeof(addr);		
	ns = connect(sd, (struct sockaddr*)&addr, addr_sz);
	if (ns<0){
		cout << strerror(errno) << endl;
		exit (-1);
	}

	pthread_create(&t_w, NULL, write_thread, NULL);

	while (true);	
	pthread_exit(NULL);
	return NULL;
}


//HERE I DO ALL THE READING. CONSTATLY.
void *write_thread(void *arg){
	int st=0;	
	int sz;
	char BUFF[512]={0};
	cout << "lmao" << endl;
	for (;;){
			cin.getline(BUFF, 512);
			st=send(sd, &BUFF, sizeof(BUFF), MSG_NOSIGNAL);
			if (st<0){
				cout << "error sending :c" << endl;
				cout << strerror(errno) << endl;			
			}
			memset(BUFF, 0, sizeof(BUFF));
	}
	
	pthread_exit(NULL);
	return NULL;
}

