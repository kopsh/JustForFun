client : client.o csapp.o
	gcc -o client client.o csapp.o -lpthread

client.o : client.c
	gcc -c client.c

csapp.o : csapp.c
	gcc -c csapp.c

clean : 
	rm -rf edit *.o
