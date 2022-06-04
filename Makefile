PROJECT     = vpr-pidjeon
VERSION     = 0.4

CMAKE       = cmake
TOOLCHAIN   = -DCMAKE_TOOLCHAIN_FILE="mingw-toolchain.cmake"
TOOLCHAIN64 = -DCMAKE_TOOLCHAIN_FILE="mingw64-toolchain.cmake"
CMAKE_FLAGS = -j$(shell nproc)

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
release: $(PROJECT)
$(PROJECT): wrapper x64 x86

wrapper:
	$(CMAKE) -B $(BUILD)/Wrapper $(TOOLCHAIN64) -DARCH="x64"
	$(CMAKE) --build $(BUILD)/Wrapper $(CMAKE_FLAGS)

x64: CMakeLists.txt
	$(CMAKE) -B $(BUILD)/x64 $(TOOLCHAIN64) -DARCH="x64"
	$(CMAKE) --build $(BUILD)/x64 $(CMAKE_FLAGS)

x86: CMakeLists.txt
	$(CMAKE) -B $(BUILD)/x86 $(TOOLCHAIN) -DARCH="x86"
	$(CMAKE) --build $(BUILD)/x86 $(CMAKE_FLAGS)

.PHONY: $(OBJECTS)
CMakeLists.txt: $(OBJECTS)
	make clean

.PHONY: install
install: wrapper x64 x86
	cp $(BIN)/$(PROJECT)-x86.exe $(BIN)/$(PROJECT)-x86
	cp $(BIN)/$(PROJECT)-x64.exe $(BIN)/$(PROJECT)-x64
	cp $(BIN)/$(PROJECT).exe $(BIN)/$(PROJECT)
	install -d $(PREFIX)/bin
	install -m 555 $(BIN)/$(PROJECT)-x86 $(PREFIX)/bin
	install -m 555 $(BIN)/$(PROJECT)-x64 $(PREFIX)/bin
	install -m 555 $(BIN)/$(PROJECT)     $(PREFIX)/bin
	rm $(BIN)/$(PROJECT)-x86
	rm $(BIN)/$(PROJECT)-x64
	rm $(BIN)/$(PROJECT)

.PHONY: release
release:
	zip $(PROJECT)-$(VERSION).zip $(BIN)/$(PROJECT)-x86.exe $(BIN)/$(PROJECT)-x64.exe $(BIN)/$(PROJECT).exe

clean:
	rm -fr ./bin/*
	rm -fr ./lib/*
	rm -fr ./build/*
	rm -f ./*.zip
	rm -f ./*log.txt
	rm -f ./temp.txt

extra-clean:
	rm -fr ./bin
	rm -fr ./lib
	rm -fr ./build
	rm -f ./*.zip
	rm -f ./*log.txt
	rm -f ./temp.txt
