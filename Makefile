TARGET = snake
CC = g++
CFLAGS = -std=c++11 ${3RD_CFLAGS}
3RD_CFLAGS = $(shell pkg-config sdl --cflags)
LDFLAGS = -Os -Wall -Wextra -lSDL -lSDL_ttf -lSDL_image

SRC := snake.cpp
#$(wildcard ${SRC}/*.cpp)
OBJ := $(notdir ${SRC:.cpp=.o})
#REL_OBJS=$(addprefix)

all: ${TARGET}

option:
	@echo  "LDFLAGS:" ${LDFLAGS}
	@echo  "CFLAGS:" ${CFLAGS}

%.o:%.cpp
	${CC} -c ${CFLAGS} $<

${TARGET}: ${OBJ}
	${CC} -o $@ ${OBJ} ${CFLAGS} ${LDFLAGS} 

global:
	@global -uv

check-syntax:
	g++ -fsyntax-only ${SRC}
	#clang -fsyntax-only -fno-color-diagnostics ${SRC}

clean:
	-rm -rf *.o ${TARGET}

.PHONY = all clean global
