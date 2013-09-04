/*************************************************************************
    > File Name: hex_tran_dd.c
    > Author: kopsh
    > Mail: kopbupt@126.com 
    > Created Time: Wed 04 Sep 2013 03:53:54 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
int main()
{
	int choice;
	struct in_addr in;
	int inip;
	char cip[16];

	while(1)
	{
		printf("1.hex to dd\n"
				"2.dd to hex\n"
				"3.exit\n"
				"Please make a choice: ");
		scanf("%d", &choice);
		
		if(choice == 3)
		{
			return 0;
		}
		else
		{
			printf("Please input a IP: ");
			if(choice == 2)
			{
				scanf("%s", cip);
				inet_aton(cip, &in);
				inip = ntohl(in.s_addr);
				printf("%x\n", inip);
			}
			else{
				scanf("%x", &inip);
				in.s_addr=htonl(inip);
				//strcpy(inet_ntoa(inip), cip);
				//printf("%s\n", cip);
				printf("%s\n", inet_ntoa(in));
			}

		}
	}
}
