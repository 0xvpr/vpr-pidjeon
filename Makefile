PROJECT     = vpr-pidjeon

CMAKE       = cmake
TOOLCHAIN   = -DCMAKE_TOOLCHAIN_FILE="mingw-toolchain.cmake"
TOOLCHAIN64 = -DCMAKE_TOOLCHAIN_FILE="mingw64-toolchain.cmake"
CMAKE_FLAGS =

BIN         = bin
BUILD       = build
SOURCE      = src
INCLUDE     = include
TEST        = src/test

SOURCES     = $(wildcard $(SOURCE)/*.c)
OBJECTS     = $(patsubst $(SOURCE)/%.c,$(BUILD)/CMakeFiles/$(PROJECT).dir/$(SOURCE)/%.c.o,$(SOURCES))

ifeq ($(PREFIX),)
PREFIX    = /usr/local
endif

all: $(PROJECT)
$(PROJECT): wrapper x64 x86

wrapper:
	$(CMAKE) -B $(BUILD)/Wrapper $(TOOLCHAIN64) -DARCH="x64"
	$(CMAKE) --build $(BUILD)/Wrapper -j$(shell nproc)

x64: CMakeLists.txt
	$(CMAKE) -B $(BUILD)/x64 $(TOOLCHAIN64) -DARCH="x64"
	$(CMAKE) --build $(BUILD)/x64 -j$(shell nproc)

x86: CMakeLists.txt
	$(CMAKE) -B $(BUILD)/x86 $(TOOLCHAIN) -DARCH="x86"
	$(CMAKE) --build $(BUILD)/x86 -j$(shell nproc)

.PHONY: $(OBJECTS)
CMakeLists.txt: $(OBJECTS)
	make clean

.PHONY: install
install: wrapper x64 x86
	cp $(BIN)/$(PROJECT)-x86.exe $(BIN)/$(PROJECT)-x86
	cp $(BIN)/$(PROJECT)-x64.exe $(BIN)/$(PROJECT)-x64
	cp $(BIN)/$(PROJECT)-x64.exe $(BIN)/$(PROJECT)
	install -d $(PREFIX)/bin
	install -m 555 $(BIN)/$(PROJECT)-x86 $(PREFIX)/bin
	install -m 555 $(BIN)/$(PROJECT)-x64 $(PREFIX)/bin
	install -m 555 $(BIN)/$(PROJECT)     $(PREFIX)/bin
	rm $(BIN)/$(PROJECT)-x86
	rm $(BIN)/$(PROJECT)-x64
	rm $(BIN)/$(PROJECT)

clean:
	rm -fr bin/*
	rm -fr build/*

extra-clean:
	rm -fr bin
	rm -fr build
