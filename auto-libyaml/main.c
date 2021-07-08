#include <yaml.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include <klee/klee.h>

#define SIZE 300
#define FIELD_SIZE 20

char YAML_SKELETON[] = "data: <symbolic_1>\nstring: <symbolic_2>\nint: <symbolic_int>\ndict: \n- key1: <symbolic_val1>\n  key2: <symbolic_val2>\n  key3: <symbolic_val3>\n- key1: <symbolic_val4>\n  key2: <symbolic_val5>\n  key3: <symbolic_val6>";

char *replace_string(char *fullstring, char *substr, char *newsubstr){

    if(!fullstring || !substr) return NULL;

    // count the number of substrs

    size_t len_substr = strlen(substr);
    size_t len_newsubstr = strlen(newsubstr);
    size_t substr_len;

    char *index;
    char *tmp = fullstring;
    int c;
    for(c = 0; index = strstr(tmp, substr); ++c){
        tmp = index + strlen(substr);
    }
    //printf("%d\n",c);
    // allocate new buffer of size 
    char *newbuf = malloc(strlen(fullstring) + (len_newsubstr - len_substr) * c + 1);
    tmp = newbuf;

    // replace substrings
    while(c--){
        index = strstr(fullstring, substr);
        substr_len = index - fullstring;
        // copy the leading chars and move the index further by the copied len
        tmp = strncpy(tmp, fullstring, substr_len) + substr_len;
        // copy new substring and move index by len of new substr
        tmp = strcpy(tmp, newsubstr) + len_newsubstr;
        fullstring += substr_len + len_substr;
    }

    // copy whatever is remaining in fullstring to tmp
    strcpy(tmp, fullstring);
    return newbuf;
}

int parser_test(){
    yaml_parser_t parser;
    yaml_token_t token;
    yaml_event_t event;
    size_t mode = 1;
    klee_make_symbolic(&mode, sizeof(mode), "mode");

    size_t keycount =0;
    size_t valcount =0;
    bool done = false;
    char *buf1, *buf2, *buf3, *buf4, *buf5, *buf6, *buf7, *buf8, *buf9 = NULL;
    char *tmp = NULL;
    buf1 = malloc(FIELD_SIZE);
    buf2 = malloc(FIELD_SIZE);
    buf3 = malloc(FIELD_SIZE);
    buf4 = malloc(FIELD_SIZE);
    buf5 = malloc(FIELD_SIZE);
    buf6 = malloc(FIELD_SIZE);
    buf7 = malloc(FIELD_SIZE);
    buf8 = malloc(FIELD_SIZE);
    buf9 = malloc(FIELD_SIZE);
    klee_make_symbolic(buf1, FIELD_SIZE, "buf1");
    klee_assume(buf1[FIELD_SIZE-1] = '\0');
    tmp = replace_string(YAML_SKELETON, "<symbolic_1>", buf1);
    klee_make_symbolic(buf2, FIELD_SIZE, "buf2");
    klee_assume(buf2[FIELD_SIZE-1] = '\0');
    tmp = replace_string(tmp, "<symbolic_2>", buf2);
    klee_make_symbolic(buf3, FIELD_SIZE, "buf3");
    klee_assume(buf3[FIELD_SIZE-1] = '\0');
    tmp = replace_string(tmp, "<symbolic_val1>", buf3);
    klee_make_symbolic(buf4, FIELD_SIZE, "buf4");
    klee_assume(buf4[FIELD_SIZE-1] = '\0');
    tmp = replace_string(tmp, "<symbolic_val2>", buf4);
    klee_make_symbolic(buf5, FIELD_SIZE, "buf5");
    klee_assume(buf5[FIELD_SIZE-1] = '\0');
    tmp = replace_string(tmp, "<symbolic_val3>", buf5);
    klee_make_symbolic(buf6, FIELD_SIZE, "buf6");
    klee_assume(buf6[FIELD_SIZE-1] = '\0');
    tmp = replace_string(tmp, "<symbolic_val4>", buf6);
    klee_make_symbolic(buf7, FIELD_SIZE, "buf7");
    klee_assume(buf7[FIELD_SIZE-1] = '\0');
    tmp = replace_string(tmp, "<symbolic_val5>", buf7);
    klee_make_symbolic(buf8, FIELD_SIZE, "buf8");
    klee_assume(buf8[FIELD_SIZE-1] = '\0');
    tmp = replace_string(tmp, "<symbolic_val6>", buf8);
    klee_make_symbolic(buf9, FIELD_SIZE, "buf9");
    klee_assume(buf9[FIELD_SIZE-1] = '\0');
    tmp = replace_string(tmp, "<symbolic_int>", buf9);

    if(!yaml_parser_initialize(&parser)) return -1;

    yaml_parser_set_input_string(&parser, tmp, strlen(tmp));

    switch(mode){
        case 1:
            do{
                if(!yaml_parser_scan(&parser, &token)){
                    printf("Failed to parse!\n");
                    return -1;
                }
                switch(token.type){
                    case YAML_KEY_TOKEN:
                        keycount++;
            //            printf("Key:\n");
                        break;
                    case YAML_VALUE_TOKEN:
                        valcount++;
            //            printf("Value:\n");
                        break;
                    case YAML_BLOCK_MAPPING_START_TOKEN:
            //            printf("Block start\n");
                        break;
                    case YAML_SCALAR_TOKEN:
            //            printf("scalar %s\n", token.data.scalar.value);
                        break;
                    default:
                        //printf("Got token of type: %d\n", token.type);
                        break;
                }
                if(token.type != YAML_STREAM_END_TOKEN) yaml_token_delete(&token);
            }while (token.type != YAML_STREAM_END_TOKEN);
            yaml_token_delete(&token);
            assert(keycount==10);
            assert(valcount==10);
            break;
        case 2:
            do{
                if(!yaml_parser_parse(&parser, &event)){
                    printf("Failed to parse!\n");
                    return -1;
                }
                switch(event.type){
                    case YAML_SCALAR_EVENT:
                        //printf("scalar %s\n", event.data.scalar.value);
                        valcount++;
                        break;
                    default:
                        //printf("Got event of type: %d\n", event.type);
                        break;
                }
                if(event.type != YAML_STREAM_END_EVENT) yaml_event_delete(&event);
            }while (event.type != YAML_STREAM_END_EVENT);
            assert(valcount==19);
            yaml_event_delete(&event);
            break; 
    }

    //printf("%d, %d", keycount, valcount);

    yaml_parser_delete(&parser);
    free(tmp);
    free(buf1);
    free(buf2);
    free(buf3);
    free(buf4);
    free(buf5);
    free(buf6);
    free(buf7);
    free(buf8);
    free(buf9);

    return 0;

}

int emitter_test(){
    yaml_emitter_t emitter;
    yaml_event_t event;

    yaml_emitter_initialize(&emitter);
    yaml_emitter_set_output_file(&emitter, stdout);

    //STREAM-START
    yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING);
    if (!yaml_emitter_emit(&emitter, &event)) return 1;

    //DOCUMENT-START
    int implicit = 0;
    klee_make_symbolic(&implicit, sizeof(implicit), "implicit");
    yaml_document_start_event_initialize(&event, NULL, NULL, NULL, implicit);
    if (!yaml_emitter_emit(&emitter, &event)) return 1;

    //Mapping start event
    yaml_mapping_start_event_initialize(&event, NULL, (yaml_char_t *)YAML_MAP_TAG,
        implicit, YAML_ANY_MAPPING_STYLE);
    if (!yaml_emitter_emit(&emitter, &event)) return 1;

    // SCALAR event
    unsigned char *buf1;
    buf1 = malloc(FIELD_SIZE);
    klee_make_symbolic(buf1, FIELD_SIZE, "value");
    buf1[FIELD_SIZE-1]='\0';
    unsigned int scalar_style = 1;
    klee_make_symbolic(&scalar_style, sizeof(scalar_style), "scalar_style");
    unsigned int quoted_implicit = 0;
    klee_make_symbolic(&quoted_implicit, sizeof(quoted_implicit), "quoted_implicit");
    unsigned int plain_implicit = 1;
    klee_make_symbolic(&plain_implicit, sizeof(plain_implicit), "plain_implicit");
    yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *)YAML_STR_TAG,
        (yaml_char_t *)buf1, strlen(buf1), plain_implicit, quoted_implicit, scalar_style);
    if (!yaml_emitter_emit(&emitter, &event)) {
        free(buf1);
        return 1;
    }
    
    //SEQUENCE_START
    unsigned int sequence_style = 0;
    klee_make_symbolic(&sequence_style, sizeof(sequence_style), "sequence_style");
    yaml_sequence_start_event_initialize(&event, NULL, (yaml_char_t *)YAML_SEQ_TAG,
       implicit, sequence_style);
    if (!yaml_emitter_emit(&emitter, &event)){
        free(buf1);
        return 1;
    }


    int count = 0;
    unsigned char *buf2;
    buf2 = malloc(FIELD_SIZE);
    klee_make_symbolic(buf2, FIELD_SIZE, "value2");
    buf2[FIELD_SIZE-1]='\0';
    unsigned char *buf3;
    buf3 = malloc(FIELD_SIZE);
    klee_make_symbolic(buf3, FIELD_SIZE, "value3");
    buf3[FIELD_SIZE-1]='\0';
    unsigned char *buf4;
    buf4 = malloc(FIELD_SIZE);
    klee_make_symbolic(buf4, FIELD_SIZE, "value4");
    buf4[FIELD_SIZE-1]='\0';
    unsigned char *buf5;
    buf5 = malloc(FIELD_SIZE);
    klee_make_symbolic(buf5, FIELD_SIZE, "value5");
    buf5[FIELD_SIZE-1]='\0';
    unsigned char *buf6;
    buf6 = malloc(FIELD_SIZE);
    klee_make_symbolic(buf6, FIELD_SIZE, "value6");
    buf6[FIELD_SIZE-1]='\0';
    unsigned int buf7 = 20;
    klee_make_symbolic(&buf7, sizeof(buf7), "value7");
    char buffer[64];
    int ret = 1;


    for (int i=0; i<6; i++) {
        yaml_mapping_start_event_initialize(&event, NULL, (yaml_char_t *)YAML_MAP_TAG,
            implicit, YAML_ANY_MAPPING_STYLE);
        if (!yaml_emitter_emit(&emitter, &event)) goto final;

        yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *)YAML_STR_TAG,
            (yaml_char_t *)buf2, strlen(buf2), plain_implicit, quoted_implicit, scalar_style);
        if (!yaml_emitter_emit(&emitter, &event)) goto final;

        yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *)YAML_STR_TAG,
            (yaml_char_t *)buf3, strlen(buf3), plain_implicit, quoted_implicit, scalar_style);
        if (!yaml_emitter_emit(&emitter, &event)) goto final;

        yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *)YAML_STR_TAG,
            (yaml_char_t *)buf4, strlen(buf4), plain_implicit, quoted_implicit, scalar_style);
        if (!yaml_emitter_emit(&emitter, &event)) goto final;

        yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *)YAML_STR_TAG,
            (yaml_char_t *)buf5, strlen(buf5), plain_implicit, quoted_implicit, scalar_style);
        if (!yaml_emitter_emit(&emitter, &event)) goto final;

        yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *)YAML_STR_TAG,
            (yaml_char_t *)buf6, strlen(buf6), plain_implicit, quoted_implicit, scalar_style);
        if (!yaml_emitter_emit(&emitter, &event)) goto final;

        sprintf(buffer, "%d", buf7);
        yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *)YAML_INT_TAG,
            (yaml_char_t *)buffer, strlen(buffer), plain_implicit, quoted_implicit, scalar_style);
        if (!yaml_emitter_emit(&emitter, &event)) goto final;

        yaml_mapping_end_event_initialize(&event);
        if (!yaml_emitter_emit(&emitter, &event)) goto final;
    }

    yaml_sequence_end_event_initialize(&event);
    if (!yaml_emitter_emit(&emitter, &event)) goto final;

    yaml_mapping_end_event_initialize(&event);
    if (!yaml_emitter_emit(&emitter, &event)) goto final;

    yaml_document_end_event_initialize(&event, 0);
    if (!yaml_emitter_emit(&emitter, &event)) goto final;

    yaml_stream_end_event_initialize(&event);
    if (!yaml_emitter_emit(&emitter, &event)) goto final;

    ret = 0;
final:
    yaml_emitter_delete(&emitter);
    free(buf1);
    free(buf2);
    free(buf3);
    free(buf4);
    free(buf5);
    free(buf6);
    return ret;
}

int main(int argc, char *argv[]){
    int mode = 1;
    klee_make_symbolic(&mode, sizeof(mode), "mode");
    switch(mode){
        case 1:
            return parser_test();
        case 2:
            return emitter_test();
        default:
            return 0;
    }
}
