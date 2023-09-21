CFLAGS= -O1 -Wall -Wno-missing-braces
LFLAGS= -lraylib -lopengl32 -lgdi32 -lwinmm
BUILD_NAME= tdgame
LIBS= -L lib/
FILES= *.c
INCLUDE= -I include/

all: compile run

compile:
	gcc $(FILES) -o $(BUILD_NAME).exe $(INCLUDE) $(LIBS) $(CFLAGS) $(LFLAGS)

run:
	.\$(BUILD_NAME).exe