include ../common.mk

SRC_PATH=./libyaml-git

INCLUDES=-I $(KLEE_SRC_DIR)/include -I $(SRC_PATH)/include -I $(SRC_PATH)
DEFINES=-D_GNU_SOURCE -DHAVE_CONFIG_H

CFLAGS=-emit-llvm -c -g $(DEFINES) $(INCLUDES)
LDFLAGS=-lgcov --coverage
CC=gcc

$(SRC_PATH)/Makefile:
	LLVM_COMPILER_PATH=$(LLVM_BUILD_DIR) LLVM_COMPILER=clang CC=$(WLLVM) ./configure

%.o: %.c
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage $< -o $@

%.bc: %.c
	LLVM_COMPILER_PATH=$(LLVM_BUILD_DIR) LLVM_COMPILER=clang $(WLLVM) $(CFLAGS) -DTEST_SYMBOLIC $< -o $@

SOURCES=$(shell ls $(SRC_PATH)/src/*.c) main.c
DEPS=$(patsubst %.c,%.o,$(SOURCES))
BC_DEPS=$(patsubst %.c,%.bc,$(SOURCES))
BC_TARGET=test-driver.bc
TARGET=test-driver-gcov

$(BC_TARGET): $(BC_DEPS)
	$(LINK) $^ -o $@
	$(OPT) -mem2reg $@ -o $@
	$(DIS) $@

$(TARGET): $(DEPS)
	$(CC) $^ -o $@ $(LDFLAGS) 

all: $(BC_TARGET)

clean:
	rm -rf $(BC_DEPS) $(DEPS) $(BC_TARGET) $(TARGET) *.bc *.ll *.gcno
