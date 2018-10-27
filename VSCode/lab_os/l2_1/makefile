
# compiler to use (gcc or g++)
CC=gcc

# final executable file name
OUT=a.out

# targets
all:
	make compile
	make clean

compile:
	$(CC) -g -c *.c
	$(CC) *.o -o $(OUT)

clean:
	rm *.o