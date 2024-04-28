PROJECT             = vpr-pidjeon

ifeq ($(CONFIG),)
CONFIG              = Release
endif

ifeq ($(VERBOSE),1)
VERBOSE             = --verbose
endif

CMAKE               = cmake
CMAKE_SOURCES      := $(shell find . -name "CMakeLists.txt")
CMAKE_TOOLCHAIN    := $(addprefix ./,mingw-toolchain.cmake)


SOURCE_DIR          = src
OBJECT_DIR          = build
BIN_DIR             = bin
LIB_DIR             = lib

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
	docker build -f "Dockerfile" -t "$(PROJECT)-dev" .
.PHONY: docker-instance
docker-instance:
	docker run -itv "$(shell pwd):/var/$(PROJECT)-dev/$(PROJECT)" -u "$(shell id -u):$(shell id -g)" "$(PROJECT)-dev"
.PHONY: docker-build
docker-build:
	docker run -v "$(shell pwd):/var/$(PROJECT)-dev/$(PROJECT)" -u "$(shell id -u):$(shell id -g)" "$(PROJECT)-dev" make "CONFIG=$(CONFIG)" 

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
