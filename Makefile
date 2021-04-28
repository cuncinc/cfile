objects = cfile.o

cfile: $(objects)
	gcc -o cfile $(objects)

cfile.o: cfile.c
	gcc -c cfile.c

clean:
	rm cfile
	rm *.o