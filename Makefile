CFLAGS= -O1 -Wall -Wno-missing-braces
LFLAGS= -lraylib -lopengl32 -lgdi32 -lwinmm
BUILD_NAME= EndlessShooter
BUILD_PATH= build/$(BUILD_NAME)
LIBS= -L lib/
FILES= *.c
INCLUDE= -I include/

all: compile run

compile:
	gcc $(FILES) -o $(BUILD_PATH).exe $(INCLUDE) $(LIBS) $(CFLAGS) $(LFLAGS)

run:
	.\$(BUILD_PATH).exe