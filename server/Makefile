objects = cfiled.o server_net.o file_system.o

cfiled: $(objects)
	gcc -o cfiled $(objects)

cfiled.o: cfiled.c server_net.h
	gcc -c cfiled.c

server_net.o: server_net.c server_net.h ../method.h file_system.h
	gcc -c server_net.c

file_system.o: file_system.c file_system.h
	gcc -c file_system.c

clean:
	rm $(objects)
	rm cfiled