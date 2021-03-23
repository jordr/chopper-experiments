include ../common.mk

SRC_PATH=./libosip-git/

INCLUDES=-I $(KLEE_SRC_DIR)/include -I $(SRC_PATH)/include
DEFINES=

COVERAGE_CFLAGS=-fprofile-arcs -ftest-coverage
COVERAGE_LDFLAGS=-lgcov --coverage

CFLAGS=$(INCLUDES) $(DEFINES) -g
LDFLAGS=
CC=gcc

TARGET=test-driver
TARGET_GCOV=test-driver-gcov
BC_TARGET=test-driver.bc

all:$(BC_TARGET)

%.o: %.c
	LLVM_COMPILER_PATH=$(LLVM_BUILD_DIR) LLVM_COMPILER=clang $(WLLVM) -c $(CFLAGS) -DTEST_SYMBOLIC $< -o $@

%.gcov.o: %.c
	$(CC) -c $(CFLAGS) $(COVERAGE_CFLAGS) $< -o $@

.PHONY: osip_archive osip_archive_gcov

libosip-git/configure: libosip-git
	@echo "Bootstrapping"
	cd libosip-git; \
		./autogen.sh

libosip-git/build/Makefile: libosip-git/configure
	@echo "Configuring $@"
	-test -d libosip-git/build || mkdir -p libosip-git/build; \
	cd libosip-git/build; \
	LLVM_COMPILER_PATH=$(LLVM_BUILD_DIR) LLVM_COMPILER=clang CC=$(WLLVM) ../configure CC=$(WLLVM) >/dev/null

osip_archive: libosip-git/build/Makefile
	@echo "Making $@"
	cd libosip-git/build/..; \
	LLVM_COMPILER_PATH=$(LLVM_BUILD_DIR) LLVM_COMPILER=clang make -C build
#	rm -rf build && mkdir build
#	cd build; LLVM_COMPILER_PATH=$(LLVM_BUILD_DIR) LLVM_COMPILER=clang ../libosip2-4.0.0/configure CC=$(WLLVM)

$(TARGET): main.o osip_archive
	LLVM_COMPILER_PATH=$(LLVM_BUILD_DIR) LLVM_COMPILER=clang $(WLLVM) $(CFLAGS) $(LDFLAGS) $< ./libosip-git/build/src/osipparser2/.libs/libosipparser2.a -o $@

$(BC_TARGET): $(TARGET) osip_archive
	LLVM_COMPILER_PATH=$(LLVM_BUILD_DIR) LLVM_COMPILER=clang $(EXTRACTBC) $< -o $@

clean:
	rm -rf $(TARGET) $(TARGET_GCOV) $(BC_TARGET) *.o *.bc *.ll libosip-git/build 
