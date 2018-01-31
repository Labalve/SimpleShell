app: main.o
main.o: main.c helpers.h
	gcc -o main.o main.c
install: app
	cp -p ./main.o ~/bin/SimpleShell;
	chmod 777 ~/bin/SimpleShell;
clean:
	-rm -f *.o
