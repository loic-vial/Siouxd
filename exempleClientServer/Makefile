all 	: client server 

client	:	client.o
	gcc -lnsl  -o client client.c 

server	:	server.o readdir.o
	gcc -lnsl  -o server server.c readdir.c

clean	: 
	rm *.o


