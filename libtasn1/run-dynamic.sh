#!/bin/bash

#KPTA=" -use-recovery-cache=1 -use-pta-mode=symbolic -use-modular-pta"

run_experiment_new()
{
echo "* Running experiment $1 (chopper-pspa)"
cd $1
# Make *.bc
make

pids=
# Make split searcher
echo "** Running split searchers"
echo "*** [cse-spa-split-searcher]"
prepare_cse cse-spa-split-searcher
make KSLICE="-use-recovery-cache=1 -use-pta-mode=static -split-search=1" all-cse
terminate_cse cse-spa-split-searcher


echo "*** [cse-pspa-split-searcher]"
prepare_cse cse-pspa-split-searcher
make KSLICE="-use-recovery-cache=1 -use-pta-mode=symbolic -use-modular-pta -split-search=1" all-cse
terminate_cse cse-pspa-split-searcher

# Make no searcher
echo "** Running no searcher in parallel"
echo "*** [cse-spa-no-searcher]"
prepare_cse cse-spa-no-searcher
make KSLICE="-use-recovery-cache=1 -use-pta-mode=static" all-cse
terminate_cse cse-spa-no-searcher

echo "*** [cse-pspa-no-searcher]"
prepare_cse cse-pspa-no-searcher
make KSLICE="-use-recovery-cache=1 -use-pta-mode=symbolic -use-modular-pta" all-cse
terminate_cse cse-pspa-no-searcher

cd ..
}

run_experiment()
{
echo "* Running experiment $1"
cd $1 
# Compiling case study
make

# Running KLEE
prepare_klee klee
make all-klee
terminate_klee klee
return

# Running CSE without CSE-crafted searcher
prepare_cse cse-no-searcher
make all-cse
terminate_cse cse-no-searcher

# Running CSE with CSE-crafted searcher
#prepare_cse cse-split-searcher-10
#make KSLICE="$KPTA -split-search -split-ratio=10" all-cse
#terminate_cse cse-split-searcher-10

prepare_cse cse-split-searcher-20
make KSLICE="$KPTA -split-search -split-ratio=20" all-cse
terminate_cse cse-split-searcher-20

# prepare_cse cse-split-searcher-30
# make KSLICE="$KPTA -split-search -split-ratio=30" all-cse
# terminate_cse cse-split-searcher-30

# prepare_cse cse-split-searcher-40
# make KSLICE="$KPTA -split-search -split-ratio=40" all-cse
# terminate_cse cse-split-searcher-40

# prepare_cse cse-split-searcher-50
# make KSLICE="$KPTA -split-search -split-ratio=50" all-cse
# terminate_cse cse-split-searcher-50

# Running CSE with CSE-optimized searcher
#prepare_cse cse-recovery-searcher-rp-10
#make KSLICE="$KPTA -recovery-search=random-path -split-ratio=10" all-cse
#terminate_cse cse-recovery-searcher-rp-10

prepare_cse cse-recovery-searcher-rp-20
make KSLICE="$KPTA -recovery-search=random-path -split-ratio=20" all-cse
terminate_cse cse-recovery-searcher-rp-20

# prepare_cse cse-recovery-searcher-rp-30
# make KSLICE="$KPTA -recovery-search=random-path -split-ratio=30" all-cse
# terminate_cse cse-recovery-searcher-rp-30

# prepare_cse cse-recovery-searcher-rp-40
# make KSLICE="$KPTA -recovery-search=random-path -split-ratio=40" all-cse
# terminate_cse cse-recovery-searcher-rp-40

# prepare_cse cse-recovery-searcher-rp-50
# make KSLICE="$KPTA -recovery-search=random-path -split-ratio=50" all-cse
# terminate_cse cse-recovery-searcher-rp-50

#prepare_cse cse-recovery-searcher-dfs-10
#make KSLICE="$KPTA -recovery-search=dfs -split-ratio=10" all-cse
#terminate_cse cse-recovery-searcher-dfs-10

prepare_cse cse-recovery-searcher-dfs-20
make KSLICE="$KPTA -recovery-search=dfs -split-ratio=20" all-cse
terminate_cse cse-recovery-searcher-dfs-20

# prepare_cse cse-recovery-searcher-dfs-30
# make KSLICE="$KPTA -recovery-search=dfs -split-ratio=30" all-cse
# terminate_cse cse-recovery-searcher-dfs-30

# prepare_cse cse-recovery-searcher-dfs-40
# make KSLICE="$KPTA -recovery-search=dfs -split-ratio=40" all-cse
# terminate_cse cse-recovery-searcher-dfs-40

# prepare_cse cse-recovery-searcher-dfs-50
# make KSLICE="$KPTA -recovery-search=dfs -split-ratio=50" all-cse
# terminate_cse cse-recovery-searcher-dfs-50

cd ..
}

terminate_klee()
{
mkdir $1
mv klee-run-* $1
mv run-klee-* $1
}

terminate_cse()
{
mkdir $1
mv cse-run-* $1
mv run-cse-* $1
grep -n "Elapsed" $1/*/info
}

prepare_klee()
{
[ -d $1 ] && rm -r $1
rm -r klee-run-random
rm -r klee-run-dfs
rm -r klee-run-coverage
}
prepare_cse()
{
[ -d $1 ] && rm -r $1
rm -r cse-run-random
rm -r cse-run-dfs
rm -r cse-run-coverage
}

## MAIN
if [ -z $1 ]
then
	run_experiment CVE-2012-1569
	run_experiment CVE-2014-3467
	run_experiment CVE-2015-2806
	run_experiment CVE-2015-3622
elif [ -d $1 ]
then
	run_experiment $1
else
	echo "$1: No such directory"
fi
