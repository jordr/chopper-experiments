#!/bin/bash

BINARY_FILE=autoklee
TIMEOUT="--max-time=3600 --max-memory=8095"

ensure_symbolic_links() {
	test -f test.bc || ln -s ../test.bc test.bc
	test -f def.asn || ln -s ../def.asn def.asn	
}

run() {
	mkdir -p auto-nurs
	cd auto-nurs
	ensure_symbolic_links
	echo Started: `date` > info.txt
	echo "chopit.py -b=$BINARY_FILE $TIMEOUT --search=nurs:covnew ${@:1}" >> info.txt
	chopit.py -b=$BINARY_FILE $TIMEOUT --search=nurs:covnew "${@:1}"
	echo Finished: `date` >> info.txt
	cat info.txt
	cd ..

	mkdir -p auto-random
	cd auto-random
	ensure_symbolic_links
	echo Started: `date` > info.txt
	echo "chopit.py -b=$BINARY_FILE $TIMEOUT --search=random-state ${@:1}" >> info.txt
	chopit.py -b=$BINARY_FILE $TIMEOUT --search=random-state "${@:1}"
	echo Finished: `date` >> info.txt
	cat info.txt
	cd ..

	mkdir -p auto-dfs
	cd auto-dfs
	ensure_symbolic_links
	echo Started: `date` > info.txt
	echo "chopit.py -b=$BINARY_FILE $TIMEOUT --search=dfs ${@:1}" >> info.txt
	chopit.py -b=$BINARY_FILE $TIMEOUT --search=dfs "${@:1}"
	echo Finished: `date` >> info.txt
	cat info.txt
	cd ..
}

: <<'START'
Benchmark to execute will start at START
START

#
cd CVE-2015-3622
run "--inline=strcmp,strlen" "--error-location=decoding.c:91" -f="asn1_get_tag_der,asn1_parser2tree,read,__fd_open -keep=_asn1_yyparse,_asn1_yylex,ioctl" "test.bc 64"
cd ..

# should be 15
cd CVE-2015-2806
run "--inline=strlen,strcat,strncat" "--error-location=parser_aux.c:574" -f="_asn1_ltostr,asn1_parser2tree,read,__user_main, -keep=_asn1_yyparse,_asn1_yylex,ioctl" "test.bc 15"
cd ..

#
cd CVE-2012-1569
run "--inline=strcmp,strlen" "--error-location=decoding.c:137" -f="asn1_get_tag_der,asn1_parser2tree,read,__user_main -keep=_asn1_yyparse,_asn1_yylex,ioctl" "test.bc 32"
cd ..

# 
cd CVE-2014-3467
mkdir -p cve1 
cd cve1
ensure_symbolic_links
run "--inline=strcmp,strlen" "--error-location=decoding.c:152" -f="asn1_get_length_ber,asn1_parser2tree,read,__user_main -keep=_asn1_yyparse,_asn1_yylex,ioctl" "test.bc 32"
cd ..
#
mkdir -p cve2 
cd cve2
ensure_symbolic_links
run "--inline=strcmp,strlen" "--error-location=decoding.c:709" -f="asn1_get_length_ber,asn1_parser2tree,read,__user_main -keep=_asn1_yyparse,_asn1_yylex,ioctl" "test.bc 32"
cd ..
#
mkdir -p cve3
cd cve3
ensure_symbolic_links
run "--inline=strcmp,strlen" "--error-location=decoding.c:1131" -f="asn1_get_length_ber,asn1_parser2tree,read,__user_main -keep=_asn1_yyparse,_asn1_yylex,ioctl" "test.bc 32"
cd ..
cd ..