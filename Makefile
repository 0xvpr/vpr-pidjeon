PROJECT = pidjeon

CC      = gcc
CFLAGS  = -std=c99 -s -O2 -Wall -Wextra -DRELEASE
CC32    = $(addprefix i686-w64-mingw32-, $(CC))
CC64    = $(addprefix x86_64-w64-mingw32-, $(CC))

CXX     = g++
CXXFLAGS = -std=c++2a -s -static -Wall -Wextra -DDEBUG
CXX32   = $(addprefix i686-w64-mingw32-, $(CXX))
CXX64   = $(addprefix x86_64-w64-mingw32-, $(CXX))

LD      = gcc
LDFLAGS =
LD32    = $(addprefix i686-w64-mingw32-, $(LD))
LD64    = $(addprefix x86_64-w64-mingw32-, $(LD))

INCLUDE = $(addprefix -I, include)

BIN     = bin
SRC     = src
OBJ     = build
32OBJ   = $(OBJ)/x86
64OBJ   = $(OBJ)/x64
SOURCES = $(wildcard $(SRC)/*.c)
32OBJS  = $(patsubst $(SRC)/%.c, $(32OBJ)/%.o, $(SOURCES))
64OBJS  = $(patsubst $(SRC)/%.c, $(64OBJ)/%.o, $(SOURCES))

TEST_TARGET   = run_tests
TEST          = tests
TESTS         = $(wildcard $(TEST)/*.c) $(filter-out %main.c,$(SOURCES))
TEST_OBJS     = $(patsubst $(TEST)/%.c, $(OBJ)/%.o, $(TESTS))
TEST_LIBDIR   = /usr/CUnit/lib
TEST_INCLUDE  = /usr/CUnit/include include
TEST_LIB      = cunit
TEST_LIBDIRS  = $(addprefix -L, $(TEST_LIBDIR))
TEST_INCLUDES = $(addprefix -I, $(TEST_INCLUDE))
TEST_LIBS     = $(addprefix -l, $(TEST_LIB))
TEST_CFLAGS   = $(CFLAGS)
TEST_LDFLAGS  = $(TEST_LIBDIRS) $(TEST_LIBS) -static

DUMMY_TARGET   = dummy
DUMMY          = $(TEST)/$(DUMMY_TARGET).cpp
32DUMMY_OBJ    = $(32OBJ)/$(DUMMY_TARGET).o
64DUMMY_OBJ    = $(64OBJ)/$(DUMMY_TARGET).o

PAYLOAD_TARGET = payload
PAYLOAD        = $(TEST)/$(PAYLOAD_TARGET).cpp
32PAYLOAD_OBJ  = $(32OBJ)/$(PAYLOAD_TARGET).o
64PAYLOAD_OBJ  = $(64OBJ)/$(PAYLOAD_TARGET).o

all: release
release: $(PROJECT)
tests: $(PROJECT) $(TEST_TARGET)

$(PROJECT): $(32OBJ) $(32OBJS) $(64OBJ) $(64OBJS) $(BIN) $(OBJ)
	$(LD32) $(32OBJS) $(LDFLAGS) -o $(BIN)/$(PROJECT)_x86.exe
	$(LD64) $(64OBJS) $(LDFLAGS) -o $(BIN)/$(PROJECT)_x64.exe

$(32OBJS): $(32OBJ)/%.o : $(SRC)/%.c
	$(CC32) $(CFLAGS) $(INCLUDE) -c $^ -o $@

$(64OBJS): $(64OBJ)/%.o : $(SRC)/%.c
	$(CC64) $(CFLAGS) $(INCLUDE) -c $^ -o $@

$(TEST_TARGET): $(DUMMY_TARGET) $(PAYLOAD_TARGET) $(BIN) $(OBJ)
	$(CC64) $(TEST_CFLAGS) $(TEST_INCLUDES) $(TESTS) $(TEST_LDFLAGS) -o $(TEST_TARGET).exe
	./run_tests.exe

$(DUMMY_TARGET): $(32DUMMY_OBJ) $(64DUMMY_OBJ)
	$(CXX32) $(CXXFLAGS) $(32DUMMY_OBJ) -o $(BIN)/$(DUMMY_TARGET)_x86.exe
	$(CXX64) $(CXXFLAGS) $(64DUMMY_OBJ) -o $(BIN)/$(DUMMY_TARGET)_x64.exe

$(32DUMMY_OBJ): $(32OBJ)
	$(CXX32) $(CXXFLAGS) -c $(DUMMY) -o $(32DUMMY_OBJ)

$(64DUMMY_OBJ): $(64OBJ)
	$(CXX64) $(CXXFLAGS) -c $(DUMMY) -o $(64DUMMY_OBJ)

$(PAYLOAD_TARGET): $(32PAYLOAD_OBJ) $(64PAYLOAD_OBJ)
	$(CXX32) $(CXXFLAGS) -shared $(32PAYLOAD_OBJ) -o $(BIN)/$(PAYLOAD_TARGET)_x86.dll
	$(CXX64) $(CXXFLAGS) -shared $(64PAYLOAD_OBJ) -o $(BIN)/$(PAYLOAD_TARGET)_x64.dll

$(32PAYLOAD_OBJ): $(32OBJ)
	$(CXX32) $(CXXFLAGS) -c $(PAYLOAD) -o $(32PAYLOAD_OBJ)

$(64PAYLOAD_OBJ): $(64OBJ)
	$(CXX64) $(CXXFLAGS) -c $(PAYLOAD) -o $(64PAYLOAD_OBJ)

$(32OBJ):
	mkdir -p $@

$(64OBJ):
	mkdir -p $@

$(BIN):
	mkdir -p $@

clean:
	rm -fr $(OBJ)/*
	rm -fr `find . -name "*.exe"`
	rm -fr `find . -name "*.dll"`
	rm -fr *log.txt
	rm -fr *temp.txt

clean_test:
	rm -fr $(OBJ)/*
	rm -fr $(TEST_TARGET).exe
	rm -fr $(DUMMY_TARGET).exe
	rm -fr $(PAYLOAD_TARGET).dll
	rm -fr *log.txt
	rm -fr *temp.txt
