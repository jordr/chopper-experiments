#!/bin/bash
if [ -z "$1" ] || [ "$1" == "--help" ]
then
	echo "usage: ./run-chopit.sh BENCHMARK|all [run|run-nurs|run-random|run-dfs] [FLAGS]"
	exit
fi

BINARY_FILE=autoklee
TIMEOUT="--max-time=3600 --max-memory=8095"
FLAGS="-b=$BINARY_FILE --dump-chopper-stats=0 $TIMEOUT "

ensure_symbolic_links() {
	test -f test.bc || ln -s ../test.bc test.bc
	test -f def.asn || ln -s ../def.asn def.asn	
}

do_chopit() {
	echo "++++++++ $(pwd) ++++++++"
	ensure_symbolic_links
	echo chopit.py $CHOPIT_FLAGS "${@:1}" > info.txt
	echo Started: `date` >> info.txt
	DATE_START=`date +%s`
	chopit.py $CHOPIT_FLAGS "${@:1}"
	echo Finished: `date` >> info.txt
	DATE_END=`date +%s`
	echo Elapsed: "$((($DATE_END-$DATE_START))) seconds" >> info.txt
	cat info.txt
	echo
}
run-nurs() {
	mkdir -p auto-nurs
	cd auto-nurs
	CHOPIT_FLAGS="$FLAGS --search=nurs:covnew"
	do_chopit "${@:1}"
	cd ..
}
run-random() {
	mkdir -p auto-random
	cd auto-random
	CHOPIT_FLAGS="$FLAGS --search=random-state"
	do_chopit "${@:1}"
	cd ..
}
run-dfs() {
	mkdir -p auto-dfs
	cd auto-dfs
	CHOPIT_FLAGS="$FLAGS --search=dfs"
	do_chopit "${@:1}"
	cd ..
}
run() {
	run-nurs "${@:1}"
	run-random "${@:1}"
	run-dfs "${@:1}"
}

: <<'START'
...
START

if [ -n "$2" ]
then
	RUN_CMD=$2
else
	RUN_CMD=run
fi

if [ -n "$3" ]
then
	echo "Adding flags ${@:3}"
	FLAGS="$FLAGS ${@:3}"
fi

if [ -n "$1" ] && [ "$1" != "all" ]
then
	echo "Running benchmark '$1'"
	if [ "$1" == "CVE-2012-1569" ]
	then
		cd $1
		$RUN_CMD "--inline=strcmp,strlen" "--error-location=decoding.c:137" -f="asn1_get_tag_der,asn1_parser2tree,read,__user_main -keep=_asn1_yyparse,_asn1_yylex,ioctl" "--args=32" "test.bc"
		cd ..
	elif [ "$1" == "CVE-2015-3622" ]
	then
	  	cd $1
		$RUN_CMD "--inline=strcmp,strlen" "--error-location=decoding.c:91" -f="asn1_get_tag_der,asn1_parser2tree,read,__fd_open -keep=_asn1_yyparse,_asn1_yylex,ioctl" "--args=64" "test.bc"
		cd ..
	elif [ "$1" == "CVE-2015-2806" ]
	then
	  	cd $1
		$RUN_CMD "--inline=strlen,strcat,strncat" "--error-location=parser_aux.c:574" -f="_asn1_ltostr,asn1_parser2tree,read,__user_main, -keep=_asn1_yyparse,_asn1_yylex,ioctl" "--args=15" "test.bc"
		cd ..
	elif [ "$1" == "CVE-2014-3467" ]
	then
		cd $1
		mkdir -p cve1; cd cve1
		$RUN_CMD "--inline=strcmp,strlen" "--error-location=decoding.c:152" -f="asn1_get_length_ber,asn1_parser2tree,read,__user_main -keep=_asn1_yyparse,_asn1_yylex,ioctl" "--args=32" "test.bc"
		cd ..
		mkdir -p cve2; cd cve2
		$RUN_CMD "--inline=strcmp,strlen" "--error-location=decoding.c:709" -f="asn1_get_length_ber,asn1_parser2tree,read,__user_main -keep=_asn1_yyparse,_asn1_yylex,ioctl" "--args=32" "test.bc"
		cd ..
		mkdir -p cve3; cd cve3
		$RUN_CMD "--inline=strcmp,strlen" "--error-location=decoding.c:1131" -f="asn1_get_length_ber,asn1_parser2tree,read,__user_main -keep=_asn1_yyparse,_asn1_yylex,ioctl" "--args=32" "test.bc"
		cd ..
		cd ..
	else
		echo "Benchmark unknown, nothing to do."
	fi
	exit
fi

echo "Running all benchmarks"
#
cd CVE-2015-3622
$RUN_CMD "--inline=strcmp,strlen" "--error-location=decoding.c:91" -f="asn1_get_tag_der,asn1_parser2tree,read,__fd_open -keep=_asn1_yyparse,_asn1_yylex,ioctl" "--args=64" "test.bc"
cd ..

# 
cd CVE-2015-2806
$RUN_CMD "--inline=strlen,strcat,strncat" "--error-location=parser_aux.c:574" -f="_asn1_ltostr,asn1_parser2tree,read,__user_main, -keep=_asn1_yyparse,_asn1_yylex,ioctl" "--args=15" "test.bc"
cd ..

#
cd CVE-2012-1569
$RUN_CMD "--inline=strcmp,strlen" "--error-location=decoding.c:137" -f="asn1_get_tag_der,asn1_parser2tree,read,__user_main -keep=_asn1_yyparse,_asn1_yylex,ioctl" "--args=32" "test.bc"
cd ..

# 
cd CVE-2014-3467
mkdir -p cve1 
cd cve1
ensure_symbolic_links
$RUN_CMD "--inline=strcmp,strlen" "--error-location=decoding.c:152" -f="asn1_get_length_ber,asn1_parser2tree,read,__user_main -keep=_asn1_yyparse,_asn1_yylex,ioctl" "--args=32" "test.bc"
cd ..
#
mkdir -p cve2 
cd cve2
ensure_symbolic_links
$RUN_CMD "--inline=strcmp,strlen" "--error-location=decoding.c:709" -f="asn1_get_length_ber,asn1_parser2tree,read,__user_main -keep=_asn1_yyparse,_asn1_yylex,ioctl" "--args=32" "test.bc"
cd ..
#
mkdir -p cve3
cd cve3
ensure_symbolic_links
$RUN_CMD "--inline=strcmp,strlen" "--error-location=decoding.c:1131" -f="asn1_get_length_ber,asn1_parser2tree,read,__user_main -keep=_asn1_yyparse,_asn1_yylex,ioctl" "--args=32" "test.bc"
cd ..
cd ..
