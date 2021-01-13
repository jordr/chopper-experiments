ROOT=$(realpath ..)
include $(ROOT)/common.mk

.SILENT:

.PHONY: all
all: bc.bc

.PHONY: clean
clean:
	rm -rf bc-git/bin/
	rm -r bytecode/

##########################################
#### Case study
##########################################
BC_DIR:=$(realpath .)/bytecode

bc.bc: bc-git bc-git/Makefile
	@echo "Compiling $@ into $(BC_DIR)"
	mkdir -p $(BC_DIR); \
	cd bc-git; \
	LLVM_COMPILER_PATH=$(LLVM_BUILD_DIR) LLVM_COMPILER=clang CFLAGS=-I/home/jruiz/code/klee-uclibc-dynamic/include/ WLLVM_BC_STORE=$(BC_DIR) CC=$(WLLVM) $(MAKE) > compilation.log 2>&1 || cat compilation.log; \
	cd ..
	@echo Linking bc files...
	$(LINK) $(BC_DIR)/* -o bc.bc
	#cd bin; \
	#LLVM_COMPILER_PATH=$(LLVM_BUILD_DIR) LLVM_COMPILER=clang CC=wllvm $(EXTRACTBC) bc

bc-git/Makefile: bc-git
	@echo "Configuring $@"
	-test -d bc-git/build || mkdir -p bc-git/build; \
	cd bc-git; \
	LLVM_COMPILER_PATH=$(LLVM_BUILD_DIR) LLVM_COMPILER=clang CC=$(WLLVM) HOSTCFLAGS= ./configure --debug > /dev/null

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
