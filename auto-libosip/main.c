#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include <klee/klee.h>

#include <osipparser2/osip_message.h>

void klee_make_symbolic(void *addr, size_t nbytes, const char *name) {

}

int test_message_parse(char *buf, size_t size){

    buf[size - 1] = 0;
    int rc;
    osip_message_t *sip;

    rc = osip_message_init(&sip);
    if (rc != 0) { 
        fprintf(stderr, "cannot allocate\n"); 
        return -1; 
    }

    rc = osip_message_parse(sip, (const char *)(buf), size);
    if (rc != 0) { 
        //fprintf(stderr, "cannot parse sip message\n"); 
    }

    osip_message_free(sip);
    free(buf);
    return rc;
}

int test_call_id(){
    osip_call_id_t *callid;
    char *a_callid = NULL;
    char *dest = NULL;
    a_callid = (char *) osip_malloc(200);
    klee_make_symbolic(a_callid, 200, "callid");
    osip_call_id_init(&callid);
    osip_call_id_parse(callid, a_callid);
    if (osip_call_id_to_str(callid, &dest) != -1){
        osip_free(dest);
    }
    osip_call_id_free(callid);
    free(a_callid);
    return 0;
}

int test_contact_id(){
    osip_contact_t *contact;
    char *a_contact = NULL;
    char *dest = NULL;
    a_contact = (char *) osip_malloc(500);
    //klee_make_symbolic(a_contact, 500, "contact");
    osip_contact_init(&contact);
    osip_contact_parse(contact, a_contact);
    if (osip_contact_to_str(contact, &dest) != -1){
        osip_free(dest);
    }
    osip_contact_free(contact);
    free(a_contact);
    return 0;
}

int test_content_type(){
    osip_content_type_t *content_type;
    char *a_content_type = NULL;
    char *dest = NULL;
    a_content_type = (char *) osip_malloc(500);
    klee_make_symbolic(a_content_type, 500, "content");
    osip_content_type_init(&content_type);
    osip_content_type_parse(content_type, a_content_type);
    if (osip_content_type_to_str(content_type, &dest) != -1){
        osip_free(dest);
    }
    osip_content_type_free(content_type);
    osip_free(a_content_type);
    return 0;
}

int test_from(){
    osip_from_t *from;
    char *a_from = NULL;
    char *dest = NULL;
    a_from = (char *) osip_malloc(200);
    klee_make_symbolic(a_from, 200, "from");
    osip_from_init(&from);
    osip_from_parse(from, a_from);
    if (osip_from_to_str(from, &dest) != -1){
        osip_free(dest);
    }
    osip_from_free(from);
    osip_free(a_from);
    return 0;
}

int test_record_route(){
    osip_record_route_t *record_route;
    char *a_record_route = NULL;
    char *dest = NULL;
    a_record_route = (char *) osip_malloc(200);
    klee_make_symbolic(a_record_route, 200, "record_route");
    osip_record_route_init(&record_route);
    osip_record_route_parse(record_route, a_record_route);
    if (osip_record_route_to_str(record_route, &dest) != -1){
        osip_free(dest);
    }
    osip_record_route_free(record_route);
    osip_free(a_record_route);
    return 0;
}

int test_tos(){
    osip_to_t *to;
    char *a_to = NULL;
    char *dest = NULL;
    a_to = (char *) osip_malloc(200);
    klee_make_symbolic(a_to, 200, "to");
    osip_to_init(&to);
    osip_to_parse(to, a_to);
    if (osip_to_to_str(to, &dest) != -1){
        osip_free(dest);
    }
    osip_to_free(to);
    osip_free(a_to);
    return 0;
}

int test_uri(){
    osip_uri_t *url;
    char *a_url = NULL;
    char *dest = NULL;
    a_url = (char *) osip_malloc(200);
    klee_make_symbolic(a_url, 200, "url");
    osip_uri_init(&url);
    osip_uri_parse(url, a_url);
    if (osip_uri_to_str(url, &dest) != -1){
        osip_free(dest);
    }
    osip_uri_free(url);
    osip_free(a_url);
    return 0;
}

int test_via(){
    osip_via_t *via;
    char *a_via = NULL;
    char *dest = NULL;
    a_via= (char *) osip_malloc(200);
    klee_make_symbolic(a_via, 200, "via");
    osip_via_init(&via);
    osip_via_parse(via, a_via);
    if (osip_via_to_str(via, &dest) != -1){
        osip_free(dest);
    }
    osip_via_free(via);
    osip_free(a_via);
    return 0;
}

int test_www_auth(){
    osip_www_authenticate_t *wwwauth;
    char *a_wwwauth = NULL;
    char *dest = NULL;
    a_wwwauth= (char *) osip_malloc(200);
    klee_make_symbolic(a_wwwauth, 200, "wwwauthenticate");
    osip_www_authenticate_init(&wwwauth);
    osip_www_authenticate_parse(wwwauth, a_wwwauth);
    if (osip_www_authenticate_to_str(wwwauth, &dest) != -1){
        osip_free(dest);
    }
    osip_www_authenticate_free(wwwauth);
    osip_free(a_wwwauth);
    return 0;
}

int main(int argc, char *argv[]) {
    int api = 1;
    klee_make_symbolic(&api, sizeof(api), "api");
    switch (api){
        case 1:
            if (argc<2){
                return 1;
            }
            size_t size = strtoul(argv[1], NULL, 10);
            char *buf = NULL;
            buf = malloc(size);
            if(argc==3){
                FILE *f=fopen(argv[2], "r");
                fread(buf, 1, size, f);
                fclose(f);
            }
            else{
#ifdef TEST_SYMBOLIC
        klee_make_symbolic(buf, size, "buf");
#endif              
            }
            return test_message_parse(buf, size);
        case 2:
            return test_call_id();
        case 3:
            return test_contact_id();
        case 4:
            return test_content_type();
        case 5:
            return test_from();
        case 6:
            return test_record_route();
        case 7:
            return test_tos();
        case 8:
            return test_uri();
        case 9:
            return test_via();
        case 10:
            return test_www_auth();
    }
    return 0;
}
