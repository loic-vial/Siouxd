all : siouxd siouxd_gps siouxd_gps_google

siouxd : siouxd.o function.o
	gcc -Wall -Wextra -lpthread -o siouxd siouxd.c function.c

siouxd_gps : siouxd_gps.o
	gcc -Wall -Wextra -o siouxd_gps siouxd_gps.c

siouxd_gps_google : siouxd_gps_google.o
	gcc -Wall -Wextra -o siouxd_gps_google siouxd_gps_google.c

clean :
	rm *.o

