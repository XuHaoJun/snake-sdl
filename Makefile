TARGET = snake
CC = g++
CFLAG  = -std=c++11
LDFLAG = -Os -Wall -Wextra -lSDL -lSDL_ttf -lSDL_image

SRC := snake.cpp
OBJ := ${SRC:.cpp=.o}

all: ${TARGET}

option:
	@echo  ${LDFLAG}

%.o:%.cpp
	${CC} -c ${CFLAG} $<

${TARGET}: ${OBJ}
	${CC} -o $@ ${OBJ} ${CFLAG} ${LDFLAG} 

global:
	@global -uv

check-syntax:
	g++ -fsyntax-only ${SRC}
	#clang -fsyntax-only -fno-color-diagnostics ${SRC}

clean:
	-rm -rf *.o ${TARGET}

.PHONY = all clean global
