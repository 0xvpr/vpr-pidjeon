PROJECT             = vpr-pidjeon

ifeq ($(CONFIG),)
CONFIG              = Release
endif

ifeq ($(VERBOSE),1)
VERBOSE             = --verbose
endif

ifeq ($(CMAKE),)
CMAKE               = cmake
endif

CMAKE_SOURCES      := $(shell find . -name "CMakeLists.txt")
CMAKE_TOOLCHAIN    := $(addprefix ./,mingw-toolchain.cmake)

SOURCE_DIR          = src
OBJECT_DIR          = build
BIN_DIR             = bin
LIB_DIR             = lib

CONTAINER           = $(PROJECT)-dev

LOCAL_UID           = $(shell id -u)
LOCAL_GID           = $(shell id -g)
LOCAL_USER          = $(shell whoami)
BUILD_ARGS          = --build-arg LOCAL_USER="$(LOCAL_USER)" --build-arg LOCAL_UID="$(LOCAL_UID)" --build-arg LOCAL_GID="$(LOCAL_GID)"

USER_SHARED_DIR     = $(shell pwd)
DOCKER_SHARED_DIR   = /var/opt/dev

OWNER               = $(LOCAL_USER):$(LOCAL_USER)
SHARED_VOLUMES      = -v "$(USER_SHARED_DIR)/bin:$(DOCKER_SHARED_DIR)/bin" -v "$(USER_SHARED_DIR)/lib:$(DOCKER_SHARED_DIR)/lib"


all: $(PROJECT)


$(PROJECT): $(OBJECT_DIR)/x86 $(OBJECT_DIR)/x64
	$(CMAKE) --build $(OBJECT_DIR)/x86 --config $(CONFIG) $(VERBOSE) 
	$(CMAKE) --build $(OBJECT_DIR)/x64 --config $(CONFIG) $(VERBOSE) 

$(OBJECT_DIR)/x86: $(shell find . -name "CMakeLists.txt") $(CMAKE_TOOLCHAIN)
	$(CMAKE) -DPROJECT_ARCHITECTURE="x86" -DCMAKE_TOOLCHAIN_FILE="$(CMAKE_TOOLCHAIN)" -B $(OBJECT_DIR)/x86

$(OBJECT_DIR)/x64: $(shell find . -name "CMakeLists.txt") $(CMAKE_TOOLCHAIN)
	$(CMAKE) -DPROJECT_ARCHITECTURE="x64" -DCMAKE_TOOLCHAIN_FILE="$(CMAKE_TOOLCHAIN)" -B $(OBJECT_DIR)/x64

.PHONY: docker-container
docker-container:
	docker build -f Dockerfile .  -t "$(CONTAINER)" $(BUILD_ARGS)

.PHONY: docker-build
docker-build:
	docker run -u "$(OWNER)" "$(CONTAINER)" /bin/bash -c "make"

.PHONY: docker-cross-compile
docker-cross-compile: $(BIN_DIR) $(LIB_DIR)
	docker run $(SHARED_VOLUMES) -u "$(OWNER)" "$(CONTAINER)" /bin/bash -c "CONFIG='$(CONFIG)' make"

.PHONY: docker-run
docker-run: $(BIN_DIR) $(LIB_DIR)
	docker run -it $(SHARED_VOLUMES) -u "$(OWNER)" "$(CONTAINER)"

.PHONY: clean
clean:
	if [ -d bin ];   then rm -fr ./bin/*;   fi
	if [ -d lib ];   then rm -fr ./lib/*;   fi 
	if [ -d build ]; then rm -fr ./build/*; fi 

.PHONY: extra-clean
extra-clean:
	rm -fr ./bin
	rm -fr ./lib
	rm -fr ./build

.PHONY: $(BIN_DIR)
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

.PHONY: $(LIB_DIR)
$(LIB_DIR):
	mkdir -p $(LIB_DIR)
