all : siouxd siouxd_gps

siouxd : siouxd.o function.o
	gcc -lpthread -o siouxd siouxd.c function.c

siouxd_gps : siouxd_gps.o
	gcc -o siouxd_gps siouxd_gps.c

clean : 
	rm *.o

