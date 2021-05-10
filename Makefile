all:
	g++ simulator.cpp *.c -o simulator -lpthread
go:
	./simulator < input.txt >> log.log
master:
	g++ simulator.cpp -o simulator_master -lpthread 
devgo:
	./simulator_master < input.txt
empty:
	./simulator
compileNEmpty:
	make all
	./simulator
run:
	make all
	./simulator < input.txt