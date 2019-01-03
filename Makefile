.PHONY: run clean

APP=wire

${APP}: main.o wire.o interface.o
	g++ -o ${APP} main.o wire.o interface.o -lsfml-graphics -lsfml-window -lsfml-system 

main.o: main.cpp
	g++ -c main.cpp

wire.o: wire.cpp wire.h
	g++ -c wire.cpp

interface.o: interface.cpp interface.h
	g++ -c interface.cpp

run: ${APP}
	./${APP}

clear:
	rm -rvf ${APP} *.o
