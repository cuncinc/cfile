objects = cfile.o config.o net.o

cfile: $(objects)
	gcc -o cfile $(objects)

cfile.o: cfile.c config.h net.h
	gcc -c cfile.c

config.o: config.c config.h
	gcc -c config.c

net.o: net.c net.h config.h
	gcc -c net.c

clean:
	rm cfile
	rm *.o