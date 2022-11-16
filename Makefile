#Makefile

main: main.o cpu.o main_class.o debug.o system.o thread.o semaphore.o
	g++ -o main main.o cpu.o main_class.o debug.o system.o thread.o semaphore.o

main.o: main.cc main_class.h
	g++ -c -o main.o main.cc

cpu.o: cpu.h cpu.cc
	g++ -c -o cpu.o cpu.cc

main_class.o: main_class.h main_class.cc
	g++ -c -o main_class.o main_class.cc

debug.o: debug.h debug.cc
	g++ -c -o debug.o debug.cc

system.o: system.h system.cc
	g++ -c -o system.o system.cc

thread.o: thread.h thread.cc
	g++ -c -o thread.o thread.cc

semaphore.o: semaphore.h semaphore.cc	
	g++ -c -o semaphore.o semaphore.cc

clean:
	rm -f main *.o