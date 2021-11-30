runclient: runclient.o client.o connection.o utils.o
	gcc -o runclient runclient.o client.o connection.o utils.o
runclient.o: runclient.c client.h connection.h 
	gcc -c runclient.c
client.o: client.c client.h utils.h utils.c
	gcc -c client.c
connection.o: connection.c connection.h
	gcc -c connection.c
utils.o: utils.c utils.h
	gcc -c utils.c