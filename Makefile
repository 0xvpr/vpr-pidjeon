PROJECT             = vpr-pidjeon

CMAKE               = /bin/cmake
CMAKE_SOURCES      := $(shell find . -name "CMakeLists.txt")
CMAKE_TOOLCHAIN    := $(addprefix ./,mingw-toolchain.cmake)


PROJECT_SOURCE_DIR := $(addprefix ./,src)
PROJECT_OBJECT_DIR := $(addprefix ./,build)
PROJECT_BIN_DIR    := $(addprefix ./,bin)
PROJECT_LIB_DIR    := $(addprefix ./,lib)


PIDJEON_NAME        = pidjeon
PIDJEON_TARGET     := $(PROJECT_BIN_DIR)/$(PROJECT)
PIDJEON_PARENT_DIR := $(PROJECT_SOURCE_DIR)/$(PIDJEON_NAME)
PIDJEON_SOURCE_DIR := $(PIDJEON_PARENT_DIR)/src
PIDJEON_SOURCES    := $(wildcard $(PIDJEON_SOURCE_DIR)/*.c)

PAYLOAD_NAME        = payload
PAYLOAD_TARGET     := $(PROJECT_LIB_DIR)/$(PAYLOAD_NAME)
PAYLOAD_PARENT_DIR := $(PROJECT_SOURCE_DIR)/$(PAYLOAD_NAME)
PAYLOAD_SOURCE_DIR := $(PAYLOAD_PARENT_DIR)/src
PAYLOAD_SOURCES    := $(wildcard $(PAYLOAD_SOURCE_DIR)/*.c)

DUMMY_NAME          = dummy
DUMMY_TARGET       := $(PROJECT_BIN_DIR)/$(DUMMY_NAME)
DUMMY_PARENT_DIR   := $(PROJECT_SOURCE_DIR)/$(DUMMY_NAME)
DUMMY_SOURCE_DIR   := $(DUMMY_PARENT_DIR)/src
DUMMY_SOURCES      := $(wildcard $(DUMMY_SOURCE_DIR)/*.c)


ALL_SOURCES        := $(PIDJEON_SOURCES) $(PAYLOAD_SOURCES) $(DUMMY_SOURCES)
ALL_OBJECTS        := $(PROJECT_OBJECT_DIR)/x86 $(PROJECT_OBJECT_DIR)/x64
ALL_TARGETS        := $(PIDJEON_TARGET).exe\
                      $(PIDJEON_TARGET)-x86.exe\
                      $(PIDJEON_TARGET)-x64.exe\
                      $(PAYLOAD_TARGET)-x86.dll\
                      $(PAYLOAD_TARGET)-x64.dll\
                      $(DUMMY_TARGET)-x86.exe\
                      $(DUMMY_TARGET)-x64.exe\
                      run-tests.exe


all: $(ALL_TARGETS)

$(ALL_TARGETS): $(ALL_SOURCES) | $(ALL_OBJECTS)
	$(CMAKE) --build $(PROJECT_OBJECT_DIR)/x86
	$(CMAKE) --build $(PROJECT_OBJECT_DIR)/x64

$(PROJECT_OBJECT_DIR)/x86: $(shell find . -name "CMakeLists.txt") $(CMAKE_TOOLCHAIN)
	$(CMAKE) -DPROJECT_ARCHITECTURE="x86" -DCMAKE_TOOLCHAIN_FILE="$(CMAKE_TOOLCHAIN)" -B $(PROJECT_OBJECT_DIR)/x86

$(PROJECT_OBJECT_DIR)/x64: $(shell find . -name "CMakeLists.txt") $(CMAKE_TOOLCHAIN)
	$(CMAKE) -DPROJECT_ARCHITECTURE="x64" -DCMAKE_TOOLCHAIN_FILE="$(CMAKE_TOOLCHAIN)" -B $(PROJECT_OBJECT_DIR)/x64


.PHONY: docker-container
docker-container:
	docker build -f "Dockerfile" -t "$(PROJECT)-dev" .
.PHONY: docker-instance
docker-instance:
	docker run -itv "$(shell pwd):/var/$(PROJECT)-dev/$(PROJECT)" -u "$(shell id -u):$(shell id -g)" "$(PROJECT)-dev"
.PHONY: docker-build
docker-build:
	docker run -v "$(shell pwd):/var/$(PROJECT)-dev/$(PROJECT)" -u "$(shell id -u):$(shell id -g)" "$(PROJECT)-dev" make


.PHONY: clean
clean:
	rm -fr `find ./bin -name "*.exe"`
	rm -fr `find ./lib -name "*.dll"`
	rm -fr `find ./build -name "*.o"`
.PHONY: extra-clean
extra-clean:
	rm -fr ./bin
	rm -fr ./lib
	rm -fr ./build
