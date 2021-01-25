#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <klee/klee.h>

#include <libtasn1.h>

#ifdef USE_ASN1_NODE
    typedef asn1_node ASN1_NODE_TYPE;
#else
    typedef ASN1_TYPE ASN1_NODE_TYPE;
#endif

int test_encoding(size_t buf_size){
    int result;
    ASN1_NODE_TYPE definitions = NULL;
    ASN1_NODE_TYPE element = NULL;
    char error[ASN1_MAX_ERROR_DESCRIPTION_SIZE];
	unsigned char data[256];
	int data_size = sizeof (data);    


    printf("Running encoding branch.\n");
    result = asn1_parser2tree("Test_encoding.asn", &definitions, error);
    if (result != ASN1_SUCCESS){
        printf("asn1_parser2tree: %s\n", error);
        return 1;        
    }
    result = asn1_create_element (definitions, "TEST_TREE.Koko", &element);
    if (result != ASN1_SUCCESS)
    {
        printf("Failed asn1_create_element()\n");
        return 1;
    }
 	result = asn1_write_value (element, "seqint", "NEW", 1);
  	if (result != ASN1_SUCCESS)
    {
      	printf("asn1_write_value(): seqint\n");
      	return 1;
    }
	char seq_val[buf_size];
    klee_make_symbolic(seq_val, buf_size,  "seq_val");
  	result = asn1_write_value (element, "seqint.?LAST", seq_val, buf_size-1);
  	if (result != ASN1_SUCCESS)
    {
      	printf("asn1_write_value(): seqint.?LAST\n");
      	return 1;
    }
	
	char int_val[buf_size];
    klee_make_symbolic(int_val, buf_size,  "int_val");
  	result = asn1_write_value (element, "int", int_val, buf_size-1);
  	if (result != ASN1_SUCCESS)
    {
      	printf("asn1_write_value(): int\n");
      	return 1;
    }
	char string[buf_size];
    klee_make_symbolic(string, buf_size,  "string");
    result = asn1_write_value (element, "str", string, buf_size-1);
    if (result != ASN1_SUCCESS){
        printf ("asn1_write_value(): str\n");
        return 1;
    }
    char bitstring[buf_size];
    klee_make_symbolic(bitstring, buf_size,  "bit_string");
    result = asn1_write_value (element, "bitstring", bitstring, buf_size-1);
    if (result != ASN1_SUCCESS){
        printf("asn1_write_value(): bit_string\n");
        return 1;
    }
   	char boolean[buf_size];
    klee_make_symbolic(boolean, buf_size,  "bool");
    result = asn1_write_value (element, "boolean", boolean, buf_size-1);
    if (result != ASN1_SUCCESS){
        printf("asn1_write_value(): boolean\n");
        return 1;
    } 
   	char any[buf_size];
    klee_make_symbolic(any, buf_size,  "any");
    result = asn1_write_value (element, "any", any, buf_size-1);
    if (result != ASN1_SUCCESS){
        printf("asn1_write_value(): any\n");
        return 1;
    } 
   	char genstring[buf_size];
    klee_make_symbolic(genstring, buf_size,  "genstring");
    result = asn1_write_value (element, "genstring", genstring, buf_size-1);
    if (result != ASN1_SUCCESS){
        printf("asn1_write_value(): genstring\n");
        return 1;
    } 
   	char time1[buf_size];
    klee_make_symbolic(time1, buf_size,  "time1");
    result = asn1_write_value (element, "time1", time1, buf_size-1);
    if (result != ASN1_SUCCESS){
        printf("asn1_write_value(): time\n");
        return 1;
    } 
    //asn1_print_structure (stdout, element, "", ASN1_PRINT_NAME_TYPE_VALUE);
    asn1_delete_structure(&definitions);
    result = asn1_der_coding(element, "", data, &data_size, NULL);
    if (result != ASN1_SUCCESS){
        printf("Encoding error.\n");
        return 1;
    }
    printf("Encoding complete.\n");
    return 0;
}
int test_decoding(size_t buf_size){
    int result;
    ASN1_NODE_TYPE definitions = NULL;
    ASN1_NODE_TYPE element = NULL;
    char error[ASN1_MAX_ERROR_DESCRIPTION_SIZE];
    unsigned char *buf = NULL;

    result = asn1_parser2tree("def.asn", &definitions, error);
    if (result != ASN1_SUCCESS) {
        printf("asn1_parser2tree: %s\n", error);
        return 1;
    }

    result = asn1_create_element(definitions, "Protocol.Message", &element);
    if (result != ASN1_SUCCESS) {
        printf("asn1_create_element: %s\n", error);
        return 1;
    }

    fprintf(stderr, "buffer size = %zu\n", buf_size);
    buf = malloc(buf_size);
    klee_make_symbolic(buf, buf_size, "buf");

    result = asn1_der_decoding(&element, buf, buf_size, error);
    if (result != ASN1_SUCCESS) {
        return 1;
    }
    printf("Decoding complete.\n");
    return 0;  
}

int run(char *argv[]){

    unsigned int api;
    size_t buf_size;
    klee_make_symbolic(&api, sizeof(api), "api");
    switch (api){
        case 1:
            buf_size = strtoul(argv[1], NULL, 10);
            return test_decoding(buf_size);
        case 2:
            buf_size = strtoul(argv[1], NULL, 10);
            return test_encoding(buf_size);
        case 3:
            // TODO: Add more tests to cover other libtasn functions
            break;
        default:
            break;
    }
    return 0;
}

int main(int argc, char *argv[], char *envp[]) {
    if (argc != 2) {
        return 1;
    }
    return run(argv);

}
