all: blk.c Simulation.cpp
	g++ -g blk.c Simulation.cpp -o Simul
	$(info Compiled into Simul)