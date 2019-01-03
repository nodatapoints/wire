.PHONY: run clean

APP=wire
GPP_FLAGS=-std=c++11
LIBRARIES=-lsfml-graphics -lsfml-window -lsfml-system

${APP}: main.o wire.o interface.o
	g++ ${GPP_FLAGS} -o ${APP} main.o wire.o interface.o ${LIBRARIES} 

main.o: main.cpp
	g++ ${GPP_FLAGS} -c main.cpp

wire.o: wire.cpp wire.h
	g++ ${GPP_FLAGS} -c wire.cpp

interface.o: interface.cpp interface.h
	g++ ${GPP_FLAGS} -c interface.cpp

run: ${APP}
	./${APP}

clear:
	rm -rvf ${APP} *.o
