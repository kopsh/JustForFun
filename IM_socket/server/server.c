/*************************************************************************
    > File Name: echoserver.c
    > Author: kopsh
    > Mail: kopbupt@126.com 
    > Created Time: Wed 09 Oct 2013 08:59:41 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"

void echo(int connfd);
int open_listenfd1(int port);
void *thread(void *vargp);
char *client_ipset[1024];
int clientfd_set[1024] = {0};
sem_t mutex;

int main(int argc, char **argv)
{
	int listenfd, *connfdp, port;
	struct sockaddr_in clientaddr;
	socklen_t clientlen = sizeof(clientaddr);
	char *haddrp;
	pthread_t tid;

	if(argc != 2){
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}
	port = atoi(argv[1]);

	listenfd = open_listenfd1(port);

	while(1)
	{
		connfdp = (int *) Malloc(sizeof(int));
		*connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		haddrp = inet_ntoa(clientaddr.sin_addr);
		client_ipset[*connfdp] = (char* )Malloc(sizeof(char*));
		strcpy(client_ipset[*connfdp], haddrp);
		Pthread_create(&tid, NULL, thread, connfdp);
	}
	exit(0);
}

//Thread routine
void *thread(void *vargp)
{
	int connfd = *((int *)vargp);//race
	
	//P(&mutex);
	clientfd_set[connfd] = 1;
	//V(&mutex);
	
	printf("server connected to %s\n", client_ipset[connfd]);
	Pthread_detach(pthread_self());
	Free(vargp);
	echo(connfd);
	printf("server disconnected to %s\n", client_ipset[connfd]);
	Close(connfd);

	//P(&mutex);
	clientfd_set[connfd] = 0;
	//V(&mutex);

	return NULL;
}

void echo(int connfd)
{
	size_t n;
	char buf[MAXLINE];
	rio_t rio;

	Rio_readinitb(&rio, connfd);
	while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
	{
		char *message = (char *)Malloc(sizeof(char *));
		strcpy(message, client_ipset[connfd]);
		strcat(message, " : ");
		strcat(message, buf);
		printf("%s", message);

		int i;
		for(i=3; i<1024; i++)
			if(clientfd_set[i] == 1 && i != connfd)
				Rio_writen(i, message, (size_t)strlen(message));
	}
}

int open_listenfd1(int port)
{
	int listenfd, optval=1;
	struct sockaddr_in serveraddr;

	//create a socket descriptor
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;
	//eliminates "Address already in use" error from bind
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0)
		return -1;

	//listenfs will be an end point for all requests to port on any IP address for this host
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short) port);
	if(bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0)
		return -1;

	//make it a listening socket ready to accept connection requests
	if(listen(listenfd, LISTENQ) < 0)
		return -1;
	return listenfd;
}
