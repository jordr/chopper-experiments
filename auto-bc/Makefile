ROOT=$(realpath ..)
include $(ROOT)/common.mk

.SILENT:

all: bc.bc

.PHONY: clean
clean:
	rm -rf bc-git/bin/
	rm -rf bytecode/
	rm -f bc.bc
	rm -f bc-git/Makefile

##########################################
#### Case study
##########################################
BC_DIR:=$(realpath .)/bytecode

bc.bc: bc-git/Makefile
	@echo "Compiling into $(BC_DIR)"
	mkdir -p $(BC_DIR)
	cd bc-git; \
	make clean &&\
	LLVM_COMPILER_PATH=$(LLVM_BUILD_DIR) LLVM_COMPILER=clang CFLAGS=-I/home/jruiz/code/klee-uclibc-dynamic/include/ WLLVM_BC_STORE=$(BC_DIR) CC=$(WLLVM) $(MAKE) 
	cd ..
	@echo Begin process to link bc files into $@
	cd $(BC_DIR); \
	../link.sh * && cp bc.bc ..
	#cd bin; \
	#LLVM_COMPILER_PATH=$(LLVM_BUILD_DIR) LLVM_COMPILER=clang CC=wllvm $(EXTRACTBC) bc

bc-git/Makefile:
	@echo "Configuring $@"
	@echo "LLVM_COMPILER_PATH=$(LLVM_BUILD_DIR) LLVM_COMPILER=clang CFLAGS=-I/home/jruiz/code/klee-uclibc-dynamic/include/ CC=$(WLLVM) HOSTCFLAGS= ./configure.sh --debug"
	-test -d bc-git/build || mkdir -p bc-git/build; \
	cd bc-git; \
	LLVM_COMPILER_PATH=$(LLVM_BUILD_DIR) LLVM_COMPILER=clang CFLAGS=-I/home/jruiz/code/klee-uclibc-dynamic/include/ CC=$(WLLVM) HOSTCFLAGS= ./configure.sh --debug

##########################################
#### Run KLEE experiments (not used)
##########################################
#BC_DIR:=$(realpath ./bc-git/build)

## Run KLEE on BC given a instruction limit
#run-klee: $(KLEE) bc.bc prepare
#	@echo "Running KLEE on program BC"
#	-taskset -c 2,3 $(KLEE) --stats-write-interval=1000 --istats-write-interval=1000 \
#	$(KFLAGS) \
#	--posix-runtime \
#	--search=random-path --search=nurs:covnew \
#	--output-dir=klee-out-bc \
#	$(BC_DIR)/bc/bc.bc \
#	A --sym-files 1 8 --sym-stdin 8 --sym-stdout

#run-cse: $(KLEE) bc.bc prepare
#	@echo "Running KLEE on program BC"
#	-taskset -c 2,3 $(KLEE) --stats-write-interval=1000 --istats-write-interval=1000 \
#	$(KFLAGS) \
	--posix-runtime \
	-split-search --search=random-state --search=nurs:covnew \
	--output-dir=cse-out-bc \
	-skip-functions=parse_args \
	$(BC_DIR)/bc/bc.bc \
	A --sym-files 1 8 --sym-stdin 8 --sym-stdout
