task06: main.o liblogger.a 
	gcc main.o -llogger -pthread -o task06 -rdynamic 

main.o: main.c
	gcc -g -Wall -Wextra -Wpedantic -std=c11 -c main.c -o main.o -D_GNU_SOURCE # -DTO_FILE

liblogger.a: logger.c
	gcc -g -Wall -Wextra -Wpedantic -std=c11 -c logger.c -o logger.o -D_GNU_SOURCE
	ar -crs liblogger.a logger.o

clear:
	rm -rf task06 *.o *.a *.log
