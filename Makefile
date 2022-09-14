#Makefile

main: main.o cpu.o main_class.o
	g++ -o main main.o cpu.o main_class.o

main.o: main.cc
	g++ -c -o main.o main.cc

cpu.o: cpu.h cpu.cc
	g++ -c -o cpu.o cpu.cc

main_class.o: main_class.h main_class.cc
	g++ -c -o main_class.o main_class.cc

clean:
	rm -f main *.o