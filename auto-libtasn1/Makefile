ROOT=$(realpath ..)
include $(ROOT)/common.mk

.SILENT:

SRC_PATH=./libtasn1-git/

INCLUDES=-I $(KLEE_SRC_DIR)/include -I $(SRC_PATH) -I $(SRC_PATH)/lib -I $(SRC_PATH)/lib/gllib
DEFINES=-D_GNU_SOURCE -DSIZEOF_UNSIGNED_INT=4 -DSIZEOF_UNSIGNED_LONG_INT=8 -DASN1_BUILDING

CFLAGS=-emit-llvm -c -g $(DEFINES) $(INCLUDES)
LDFLAGS=

%.bc: %.c
	echo -e "\e[0;31m [%$@]\e[0;m\
		Running \e[0;32m$(CLANG) $(CFLAGS) $< -o $@\e[0;m"
	$(CLANG) $(CFLAGS) $< -o $@

# SRC_MAIN=driver.c
# TARGET=test-driver.bc
SRC_MAIN=main.c
TARGET=test.bc

SOURCES=$(shell ls $(SRC_PATH)/lib/*.c) $(SRC_MAIN)
DEPS=$(patsubst %.c,%.o,$(SOURCES))
BC_DEPS=$(patsubst %.c,%.bc,$(SOURCES))

TIMEOUT_FLAGS=--max-time=3600 --max-memory=8095

all: $(TARGET)

$(TARGET): $(BC_DEPS)
	echo -e "\e[0;31m [%$(TARGET)!]\e[0;m\
		Running \e[0;32m\"$(BC_DEPS)\n\
		$(LINK) $^ -o $@\n\
		$(OPT) -mem2reg $@ -o $@\n\
		$(DIS) $@\"\e[0;m"
	$(LINK) $^ -o $@
	$(OPT) -mem2reg $@ -o $@
	$(DIS) $@

clean:
	rm -rf $(DEPS) $(BC_DEPS) $(TARGET)
	rm -f test.* *.bc
#	rm -rf klee-*
