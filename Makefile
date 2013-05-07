TARGET = snake
CC = g++
LDFLAG = -Os -Wall -Wextra -lSDL -lSDL_ttf -lSDL_image

SRC := snake.cpp
OBJ := ${SRC:.cpp=.o}

all: ${TARGET}

option:
	@echo  ${LDFLAG}

%.o:%.cpp
	${CC} -c $<

${TARGET}: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAG}

global:
	@global -uv

check-syntax:
	g++ -fsyntax-only ${SRC}
	#clang -fsyntax-only -fno-color-diagnostics ${SRC}

clean:
	-rm -rf *.o ${TARGET} GPATH GTAGS GRTAGS

.PHONY = all clean global
