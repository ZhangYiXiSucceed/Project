test:test.o Add.o hello.o print.o 
	gcc -o test test.o Add.o hello.o print.o 
test.o: test.c Add.h hello.h print.h
	gcc -c test.c -o test.o
Add.o: Add.c Add.h
	gcc -c Add.c -o Add.o
hello.o: hello.c hello.h
	gcc -c hello.c -o hello.o
print.o: print.c print.h
	gcc -c print.c -o print.o
clean:
	rm *.o test
