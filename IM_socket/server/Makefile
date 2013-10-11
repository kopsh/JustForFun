server : server.o csapp.o 
	gcc -o server server.o csapp.o -pthread

server.o : server.c csapp.h
	gcc -c server.c 

csapp.o : csapp.c csapp.h
	gcc -c csapp.c 
