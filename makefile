# this makefile is ran from the root directory of the project

CC=gcc
CXX=g++
CXXOPTS=-Wall -O0 -g -std=c++11 -DDEBUG
COPTS=-Wall -O0 -g -std=c11
CPPFLAGS=-I./ -I/usr/include -I./include
CFLAGS=-Wall -O0 -g -std=c11 -DDEBUG
AR=ar

#two exectuables are created, eSim and basicTest
#  eSim is the elevator simulator
#  basicTest is an offline test program
all: basicTest eSim

# create the eSim program
src/elevatorGUI.cxx: src/elevatorGUI.fl
	fluid -o src/elevatorGUI.cxx -h src/elevatorGUI.h -c src/elevatorGUI.fl

# build the library of C files
CFILES= \
          ./elevatorController/elevatorController.c \
          ./src/elevator.c \
          ./src/events.c 

OBJFILES=${CFILES:.c=.o}

libevsim.a: ${OBJFILES}
	${AR} rcs libevsim.a ${OBJFILES}
  

eSim: src/elevatorGUI.cxx src/main.cxx libevsim.a
	${CXX} ${CPPFLAGS} \
		src/elevatorGUI.cxx  src/main.cxx   \
	 	-o eSim ${CXXOPTS} -L./ -levsim -lfltk

basicTest: elevatorController/basicTest.c libevsim.a
	${CC} ${CPPFLAGS} \
		elevatorController/basicTest.c \
		-o basicTest ${COPTS} -L./ -levsim

basicElevatorTest: src/basicElevatorTest.c libevsim.a
	${CC} ${CPPFLAGS} \
		src/basicElevatorTest.c \
		-o basicElevatorTest ${COPTS} -L./ -levsim

test: basicTest basicElevatorTest #controllerTest1
	./basicTest
	./basicElevatorTest

clean:
	-rm -f src/elevatorGUI.cxx src/elevatorGUI.h eSim \
		${OBJFILES} libevsim.a \
		elevatorController/basicTest.o \
		basicTest controllerTest1 basicElevatorTest
