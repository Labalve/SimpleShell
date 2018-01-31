app: main.o
main.o: main.c helpers.h
	gcc -o main.o main.c
install: app
	cp -p ./main.o final/SimpleShell;
	chmod 777 final/SimpleShell;
clean:
	-rm -f *.o
