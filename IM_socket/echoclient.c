/*************************************************************************
    > File Name: echoclient.c
    > Author: kopsh
    > Mail: kopbupt@126.com 
    > Created Time: Tue 08 Oct 2013 10:31:15 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"

int open_clientfd1(const char *, int);
void *thread(void *vargp);

int main(int argc, char **argv) 
{
	int clientfd, port;
	char *ip, buf[MAXLINE];
	rio_t rio;
	pthread_t tid;


	if(argc != 3)
	{
		fprintf(stderr, "usage: %s <ip> <post>\n", argv[0]);
		exit(0);
	}
	ip = argv[1];
	port = atoi(argv[2]);

	clientfd = open_clientfd1(ip, port);
	rio_readinitb((rio_t *)&rio,(int) clientfd);

	Pthread_create(&tid, NULL, thread, &clientfd);

	while(Fgets(buf, MAXLINE, stdin) != NULL)
	{
		Rio_writen(clientfd, buf, strlen(buf));
		//Rio_readlineb(&rio, buf, MAXLINE);
		//Fputs(buf, stdout);
	}
	close(clientfd);
}

void *thread(void *vargp)
{
	int connfd = *((int *)vargp);
	size_t n;
	rio_t rio;
	char buf[MAXLINE];

	Rio_readinitb(&rio, connfd);

	while(1)
	{
		while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
		{
			printf("hehe\n");
			printf("%s", buf);	
		}
	}
}

int open_clientfd1(const char* ip, int port)
{
	int clientfd;
	struct sockaddr_in serveraddr;

	//Check errno for cause of error
	if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);//network byte order(大端字节顺序)
	inet_aton(ip, &serveraddr.sin_addr);//ip transform
	
	if(connect(clientfd, (SA *) &serveraddr, sizeof(serveraddr)) < 0)
		return -1;
	return clientfd;
}

