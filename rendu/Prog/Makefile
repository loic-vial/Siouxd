all : siouxd siouxd_gps

siouxd : siouxd.o function.o
	gcc -g -lpthread -o siouxd siouxd.c function.c

siouxd_gps : siouxd_gps.o
	gcc -g -o siouxd_gps siouxd_gps.c

clean : 
	rm *.o

