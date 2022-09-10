#Makefile

main:
	g++ -o main cpu.cc main.cc main_class.cc

clean:
	rm -f main *.o