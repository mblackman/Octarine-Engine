CC = g++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors
INCLUDE_PATH = -I "./libs"
SRC_FILES = ./src/*.cpp \
			./src/Game/*.cpp \
			./src/General/*.cpp \
			./src/ECS/*.cpp \
			./src/AssetManager/*.cpp \
			./src/MapEditor/*.cpp \
			./src/Renderer/*.cpp \
			./libs/imgui/*.cpp 
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.3
OBJ_NAME = bin/gameengine

debug:
	mkdir -p bin
	${CC} ${LANG_STD} ${COMPILER_FLAGS} ${INCLUDE_PATH} ${SRC_FILES} ${LINKER_FLAGS} -g -o ${OBJ_NAME}

build:
	mkdir -p bin
	${CC} ${LANG_STD} ${COMPILER_FLAGS} ${INCLUDE_PATH} ${SRC_FILES} ${LINKER_FLAGS} -o ${OBJ_NAME}

run:
	./${OBJ_NAME}

run-map:
	./${OBJ_NAME} -m

clean:
	rm -rf bin/