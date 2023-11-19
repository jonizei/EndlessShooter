DEBUG= 0
BUILD= 0
CFLAGS= -Wall -Wno-missing-braces
LFLAGS= -lraylib -lopengl32 -lgdi32 -lwinmm
BUILD_NAME= EndlessShooter
BUILD_DIR= build
BUILD_PATH= $(BUILD_DIR)/$(BUILD_NAME)
LIBS= -L lib/
FILES= *.c
INCLUDE= -I include/
RUN_COMMAND ?= .\$(BUILD_PATH).exe
BUILD_COMMAND ?= 
RESOURCE_DIR = resources
OBJECTS ?= compile run

ifeq ($(DEBUG), 1)
	CFLAGS += -g
	RUN_COMMAND = gdb $(BUILD_PATH).exe
else
	CFLAGS += -O1
endif

ifeq ($(BUILD), 1)
	OBJECTS = compile
endif

all: $(OBJECTS)

compile:
ifeq ($(BUILD), 1)
	gcc $(FILES) -o $(BUILD_PATH).exe $(INCLUDE) $(LIBS) $(CFLAGS) $(LFLAGS)
	xcopy /y /e /i "./$(RESOURCE_DIR)" "./$(BUILD_DIR)/$(RESOURCE_DIR)"
else
	gcc $(FILES) -o $(BUILD_PATH).exe $(INCLUDE) $(LIBS) $(CFLAGS) $(LFLAGS)
endif

run:
	$(RUN_COMMAND)