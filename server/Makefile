objects = cfiled.o

cfiled: $(objects)
	gcc -o cfiled $(objects)

cfiled.o: cfiled.c log.h
	gcc -c cfiled.c

clean:
	rm $(objects)
	rm cfiled