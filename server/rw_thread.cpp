#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <iostream>
#include <semaphore.h>
using namespace std;

//global variables
extern pthread_t t[10], t_r[10];
extern sem_t sem;
extern int sd;
extern int addr_sz;

// thread functions
void *read_thread(void *arg);

// structs
struct operation{
	int new_socket;
	bool is_dead;
	bool do_kill;
} ;
extern struct sockaddr_in addr;
extern struct sockaddr_storage addr_s;


void *handler(void *arg){
	struct operation o;
	int ns=-1;

	//LISTENS FOR A SOCKET UNTIL IT IS ACCEPTED
	sem_wait(&sem);
		while (ns<0){
			addr_sz = sizeof(addr_s);		
			ns = accept(sd, (struct sockaddr*)&addr_s, (socklen_t*)&addr_sz);
			sleep(1);
		}
	sem_post(&sem);

	//HERE I INITIALIZE THE STRUCT THAT WILL BE PASSED TO THE READ THREAD
	//THE STRUCT HAS IS_DEAD AND DO_KILL
	//THEY DICTATE IF I SHOULD RESTART THE SOCKET OR STOP THE SERVER
	//i IS THE THREAD ID NUMBER
	//NEW_SOCK IS THE CURENT SOCK HANDLE
	int i= *(int*)arg;
	o.new_socket = ns;
	o.is_dead = false;
	o.do_kill = false;	
	pthread_create(&t_r[i], NULL, read_thread, &o);

	//WHILE THE SOCKET IS ALIVE KEEP WAIT
	while (o.is_dead == false) ;


	//KILL THE THREAD AND THE CURENT SOCKET
	pthread_cancel(t_r[i]);
	close(ns);

	cout << "Thread/Socket nr[" << i << "] has died." << endl;
	
	pthread_exit(NULL);
	return NULL;
}


//HERE I DO ALL THE READING. CONSTATLY.
void *read_thread(void *arg){
	struct operation *o = (struct operation*)arg;

	int st=0;	
	int sz;
	char BUFF[512]={0};
	unsigned short ping=0;
	for (;;){
			//ping the client to see if its alive
			st=send(o->new_socket, &ping, sizeof(ping), MSG_NOSIGNAL);
			if (st<0){
				o -> is_dead = true;
				break;
			}



			recv(o->new_socket, &BUFF, sizeof(BUFF), MSG_NOSIGNAL);
			
			sz=strlen(BUFF);
			if (BUFF[0] != 0){
				if (BUFF[sz-1] != '\n'){
					BUFF[sz] = '\n';
					BUFF[sz+1] = '\0';	
				}
				printf("\t\033[33m:%s\033[0m", BUFF);
				//fflush(stdout);	
				memset(BUFF, 0, sizeof(BUFF));
			}
	}
	
	pthread_exit(NULL);
	return NULL;
}

