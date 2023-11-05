DEBUG= 0
CFLAGS= -Wall -Wno-missing-braces
LFLAGS= -lraylib -lopengl32 -lgdi32 -lwinmm
BUILD_NAME= EndlessShooter
BUILD_PATH= build/$(BUILD_NAME)
LIBS= -L lib/
FILES= *.c
INCLUDE= -I include/
RUN_COMMAND ?= .\$(BUILD_PATH).exe

ifeq ($(DEBUG), 1)
	CFLAGS += -g
	RUN_COMMAND = gdb $(BUILD_PATH).exe
else
	CFLAGS += -O1
endif

all: compile run

compile:
	gcc $(FILES) -o $(BUILD_PATH).exe $(INCLUDE) $(LIBS) $(CFLAGS) $(LFLAGS)

run:
	$(RUN_COMMAND)