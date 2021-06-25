build:
	gcc ImplementingAlgos.h
	gcc process_generator.c -o process_generator.out
	gcc clk.c -o clk.o
	gcc scheduler.c -o scheduler.o -lm
	gcc process.c -o process.o
	gcc test_generator.c -o test_generator.out

clean:
	rm -f *.out  processes.txt

all: clean build

run:
	
	./process_generator.out
