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

#define SIZE 200
#define VAL_SIZE 100

typedef struct
{
  int lineNumber;
  char *line;
  int errorNumber;
  char *errorDescription;
} test_type;

char fileCorrectName[] = "Test_parser.asn";
char fileErroredName[] = "Test_parser_ERROR.asn";

int
readLine (FILE *file, char *line)
{
  int c;
  while (((c = fgetc(file)) != EOF) && ( c != 10))
    {
        *line = c;
        line++;
    }

  *line = 0;

  return c;
}

void
createFile (int lineNumber, char *line)
{
  FILE *fileIn, *fileOut;
  char lineRead[1024];
  int fileInLineNumber = 0;

  fileIn = fopen(fileCorrectName, "r");
  if (fileIn == NULL) printf("Failed to read %s\n", fileCorrectName); 
  fileOut = fopen(fileErroredName, "w");
  if (fileOut == NULL) printf("Failedto open for write %s\n", fileErroredName);

  while (readLine (fileIn, lineRead) != EOF)
    {
      fileInLineNumber++;
      printf("Line Number: %d\n", fileInLineNumber);
      printf("Target lineNumber: %d\n", lineNumber);
      if (fileInLineNumber == lineNumber){
          fprintf (fileOut, "%s\n", line);
      } else { 
          fprintf (fileOut, "%s\n", lineRead);
      }
    }

  fclose (fileOut);
  fclose (fileIn);
}

int test_parser(){
    asn1_retCode result;
    ASN1_TYPE definitions = ASN1_TYPE_EMPTY;
    char errorDescription[ASN1_MAX_ERROR_DESCRIPTION_SIZE];
    test_type *test;
    int errorCounter = 0, testCounter = 0;
    char *buf1, *buf2, *buf3, *buf4;
    buf1 = malloc(SIZE);
    buf2 = malloc(SIZE);
    buf3 = malloc(SIZE);
    buf4 = malloc(SIZE);
    klee_make_symbolic(buf1, SIZE, "buf");
    klee_assume(buf1[VAL_SIZE-1] == '\0');
    klee_make_symbolic(buf2, SIZE, "buf");
    klee_assume(buf2[VAL_SIZE-1] == '\0');
    klee_make_symbolic(buf3, SIZE, "buf");
    klee_assume(buf3[VAL_SIZE-1] == '\0');
    klee_make_symbolic(buf4, SIZE, "buf");
    klee_assume(buf4[VAL_SIZE-1] == '\0');
    test_type test_array[] = {
        {12, buf1, ASN1_SUCCESS, ""},
        {12, buf2, ASN1_NAME_TOO_LONG, ""},
        {16, buf3, ASN1_IDENTIFIER_NOT_FOUND, ""},
        {20, buf4, ASN1_SYNTAX_ERROR, ""},
        {0}
    };
    
    test = test_array;
    while (test->lineNumber !=0 ){
        testCounter++;
        printf("creating File.\n");
        createFile(test->lineNumber, test->line);
        result = asn1_parser2tree (fileErroredName, &definitions, errorDescription);
        asn1_delete_structure (&definitions);
        if ((result != test->errorNumber) || (strcmp (errorDescription, test->errorDescription))){
	        errorCounter++;
	    }
        test++;
    }
    printf("Finished Creation");
    free(buf1);
    free(buf2);
    free(buf3);
    free(buf4);
    return 0;
}

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
    free(buf);
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
            return test_parser();
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
