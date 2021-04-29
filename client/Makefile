objects = cfile.o config.o

cfile: $(objects)
	gcc -o cfile $(objects)

cfile.o: cfile.c config.h
	gcc -c cfile.c

config.o: config.c config.h
	gcc -c config.c

clean:
	rm cfile
	rm *.o