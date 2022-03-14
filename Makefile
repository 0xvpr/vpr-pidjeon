PROJECT     = pidjeon

CMAKE       = cmake
TOOLCHAIN   = -DCMAKE_TOOLCHAIN_FILE="mingw-toolchain.cmake"
TOOLCHAIN64 = -DCMAKE_TOOLCHAIN_FILE="mingw64-toolchain.cmake"
CMAKE_FLAGS =

BIN         = bin
BUILD       = build
SOURCE      = src
INCLUDE     = include
TEST        = test

SOURCES     = $(wildcard $(SOURCE)/*.c)
OBJECTS     = $(patsubst $(SOURCE)/%.c,$(BUILD)/CMakeFiles/$(PROJECT).dir/$(SOURCE)/%.c.o,$(SOURCES))

MAKEFLAGS  += -j$(shell nproc)

all: $(PROJECT)
$(PROJECT): x64 x86

x64: CMakeLists.txt
	$(CMAKE) -B $(BUILD)/x64 $(TOOLCHAIN64) -DARCH="x64"
	$(CMAKE) --build $(BUILD)/x64

x86: CMakeLists.txt
	$(CMAKE) -B $(BUILD) $(TOOLCHAIN) -DARCH="x86"
	$(CMAKE) --build $(BUILD)

.PHONY: $(OBJECTS)
CMakeLists.txt: $(OBJECTS)
	make clean

clean:
	rm -fr bin/*
	rm -fr build/*

extra-clean:
	rm -fr bin
	rm -fr build
