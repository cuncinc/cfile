objects = cfile.o config.o client_net.o

cfile: $(objects)
	gcc -o cfile $(objects)

cfile.o: cfile.c config.h client_net.h ../method.h
	gcc -c cfile.c

config.o: config.c config.h
	gcc -c config.c

client_net.o: client_net.c client_net.h config.h ../method.h
	gcc -c client_net.c

clean:
	rm *.o