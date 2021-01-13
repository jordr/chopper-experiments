#!/bin/bash
#Must be executed in bytecode dir

files=( "$@" )
main_files=( )

for i in $@
do
	if ~/code/llvm-38-build_O_D_A/bin/llvm-dis -f -o - $i | grep -q @main; then
		echo "[link.sh 1/3] $i contains @main symbol."
		files=("${files[@]/$i}") 
		main_files+=("$i")
	fi
done
for mainf in "${main_files[@]}"
do
	echo "[link.sh 2/3] linking with $mainf"
	~/code/llvm-38-build_O_D_A/bin/llvm-link ${files[@]} $mainf -o $mainf.bc
	if ~/code/llvm-38-build_O_D_A/bin/lli $mainf.bc --help | grep -q "GNU bc"; then
		echo "[link.sh 3/3] identified $mainf.bc as bc.bc"
		cp $mainf.bc bc.bc
		exit 0
	fi
done
exit 1
