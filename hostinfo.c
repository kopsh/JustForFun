/*************************************************************************
    > File Name: hostinfo.c
    > Author: kopsh
    > Mail: kopbupt@126.com 
    > Created Time: Wed 04 Sep 2013 09:41:05 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
	char **pp;
	struct in_addr addr;
	struct hostent *hostp;

	if(argc != 2){
		fprintf(stderr, "usage: %s <domain name or dotted-decimal>\n",
				argv[0]);
		exit(0);
	}

	if(inet_aton(argv[1], &addr))
		hostp = gethostbyaddr((const char *) &addr, sizeof(addr), AF_INET);
	else
		hostp = gethostbyname(argv[1]);

	printf("official hostname: %s\n", hostp->h_name);
	printf("host address type: %d\n", hostp->h_addrtype);
	printf("length of address: %d\n", hostp->h_length);

	for(pp = hostp->h_aliases; *pp !=NULL; pp++)
		printf("alias: %s\n", *pp);

	for(pp= hostp->h_addr_list; *pp != NULL; pp++){
		addr.s_addr =((struct in_addr *) *pp)->s_addr;
		printf("address: %s\n", inet_ntoa(addr));
	}
	exit(0);
}
