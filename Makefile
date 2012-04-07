default:
	g++ -Wall -pedantic -g -o plot plot.cpp `libpng-config --ldflags`
	g++ -Wall -pedantic -g -o manifold manifold.cpp

serial:
	g++ -Wall -pedantic -g -o bot eggbot.cpp -lserial
