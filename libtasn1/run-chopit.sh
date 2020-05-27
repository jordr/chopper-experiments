#
cd CVE-2015-3622
cd nurs
date > nurs.txt
chopit.py -b=autokleepspa --inline=strcmp,strlen --search=nurs:covnew -c=" --error-location=decoding.c:91 --max-time=3600 --max-memory=8095 " -f="asn1_get_tag_der,asn1_parser2tree,read,__fd_open,_asn1_yylex" "test.bc 64"
cd ..
cd random
date > random.txt
chopit.py -b=autokleepspa --inline=strcmp,strlen --search=random-state -c=" --error-location=decoding.c:91 --max-time=3600 --max-memory=8095 " -f="asn1_get_tag_der,asn1_parser2tree,read,__fd_open,_asn1_yylex" "test.bc 64"
cd ..
cd dfs
date > dfs.txt
chopit.py -b=autokleepspa --inline=strcmp,strlen --search=dfs -c=" --error-location=decoding.c:91 --max-time=3600 --max-memory=8095 " -f="asn1_get_tag_der,asn1_parser2tree,read,__fd_open,_asn1_yylex -keep=asn1_parser2tree,_asn1_yyparse,_asn1_yylex" "test.bc 64"
cd ..
cd ..
# should be 15
cd CVE-2015-2806
cd nurs
date > nurs.txt
chopit.py -b=autokleepspa --inline=strlen,strcat,strncat --search=nurs:covnew -c=" --error-location=parser_aux.c:574 --max-time=3600 --max-memory=8095 " -f="_asn1_ltostr,read,syscall -keep=asn1_parser2tree,_asn1_yyparse,_asn1_yylex" "test.bc 32"
cd ..
cd random
date > random.txt
chopit.py -b=autokleepspa --inline=strlen,strcat,strncat --search=random-state -c=" --error-location=parser_aux.c:574 --max-time=3600 --max-memory=8095 " -f="_asn1_ltostr,read,syscall -keep=asn1_parser2tree,_asn1_yyparse,_asn1_yylex" "test.bc 32"
cd ..
cd dfs
date > dfs.txt
chopit.py -b=autokleepspa --inline=strlen,strcat,strncat --search=dfs -c=" --error-location=parser_aux.c:574 --max-time=3600 --max-memory=8095 " -f="_asn1_ltostr,read,syscall -keep=asn1_parser2tree,_asn1_yyparse,_asn1_yylex" "test.bc 32"
cd ..
cd ..
#
cd CVE-2012-1569
cd nurs
date > nurs.txt
chopit.py -b=autokleepspa --inline=strcmp,strlen --search=nurs:covnew -c=" --error-location=decoding.c:137 --max-time=3600 --max-memory=8095 " -f="asn1_get_tag_der,asn1_parser2tree,read -keep=asn1_parser2tree,_asn1_yyparse,_asn1_yylex" "test.bc 32"
cd ..
cd random
date > random.txt
chopit.py -b=autokleepspa --inline=strcmp,strlen --search=random-state -c=" --error-location=decoding.c:137 --max-time=3600 --max-memory=8095 " -f="asn1_get_tag_der,asn1_parser2tree,read -keep=asn1_parser2tree,_asn1_yyparse,_asn1_yylex" "test.bc 32"
cd ..
cd dfs
date > dfs.txt
chopit.py -b=autokleepspa --inline=strcmp,strlen --search=dfs -c=" --error-location=decoding.c:137 --max-time=3600 --max-memory=8095 " -f="asn1_get_tag_der,asn1_parser2tree,read -keep=asn1_parser2tree,_asn1_yyparse,_asn1_yylex" "test.bc 32"
cd ..
cd ..
# max time changed
cd CVE-2014-3467
cd nurs-1
date > nurs.txt
chopit.py -b=autokleepspa --inline=strcmp,strlen --search=nurs:covnew -c=" --error-location=decoding.c:152 --max-time=3600 --max-memory=8095 " -f="asn1_get_length_ber,asn1_parser2tree,read -keep=asn1_parser2tree,_asn1_yyparse,_asn1_yylex" "test.bc 32"
cd ..
cd random-1
date > random.txt
chopit.py -b=autokleepspa --inline=strcmp,strlen --search=random-state -c=" --error-location=decoding.c:152 --max-time=3600 --max-memory=8095 " -f="asn1_get_length_ber,asn1_parser2tree,read -keep=asn1_parser2tree,_asn1_yyparse,_asn1_yylex" "test.bc 32"
cd ..
cd dfs-1
date > dfs.txt
chopit.py -b=autokleepspa --inline=strcmp,strlen --search=dfs -c=" --error-location=decoding.c:152 --max-time=3600 --max-memory=8095 " -f="asn1_get_length_ber,asn1_parser2tree,read -keep=asn1_parser2tree,_asn1_yyparse,_asn1_yylex" "test.bc 32"
cd ..
#
cd nurs-2
date > nurs.txt
chopit.py -b=autokleepspa --inline=strcmp,strlen --search=nurs:covnew -c=" --error-location=decoding.c:709 --max-time=3600 --max-memory=8095 " -f="_asn1_get_octet_string,asn1_parser2tree,read -keep=asn1_parser2tree,_asn1_yyparse,_asn1_yylex" "test.bc 32"
cd ..
cd random-2
date > random.txt
chopit.py -b=autokleepspa --inline=strcmp,strlen --search=random-state -c=" --error-location=decoding.c:709 --max-time=3600 --max-memory=8095 " -f="_asn1_get_octet_string,asn1_parser2tree,read -keep=asn1_parser2tree,_asn1_yyparse,_asn1_yylex" "test.bc 32"
cd ..
cd dfs-2
date > dfs.txt
chopit.py -b=autokleepspa --inline=strcmp,strlen --search=dfs -c=" --error-location=decoding.c:709 --max-time=3600 --max-memory=8095 " -f="_asn1_get_octet_string,asn1_parser2tree,read -keep=asn1_parser2tree,_asn1_yyparse,_asn1_yylex" "test.bc 32"
cd ..
#
cd nurs-3
date > nurs.txt
chopit.py -b=autokleepspa --inline=strcmp,strlen --search=nurs:covnew -c=" --error-location=decoding.c:1131 --max-time=3600 --max-memory=8095 " -f="asn1_der_decoding_element,asn1_parser2tree,read -keep=asn1_parser2tree,_asn1_yyparse,_asn1_yylex" "test.bc 32"
cd ..
cd random-3
date > random.txt
chopit.py -b=autokleepspa --inline=strcmp,strlen --search=random-state -c=" --error-location=decoding.c:1131 --max-time=3600 --max-memory=8095 " -f="asn1_der_decoding_element,asn1_parser2tree,read -keep=asn1_parser2tree,_asn1_yyparse,_asn1_yylex" "test.bc 32"
cd ..
cd dfs-3
date > dfs.txt
chopit.py -b=autokleepspa --inline=strcmp,strlen --search=dfs -c=" --error-location=decoding.c:1131 --max-time=3600 --max-memory=8095 " -f="asn1_der_decoding_element,asn1_parser2tree,read -keep=asn1_parser2tree,_asn1_yyparse,_asn1_yylex" "test.bc 32"
cd ..
cd ..
