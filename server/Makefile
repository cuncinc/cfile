objects = cfiled.o server_net.o

cfiled: $(objects)
	gcc -o cfiled $(objects)

cfiled.o: cfiled.c server_net.h
	gcc -c cfiled.c

server_net.o: server_net.c server_net.h ../method.h
	gcc -c server_net.c

clean:
	rm $(objects)
	rm cfiled