#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#define EVA_VALUE_TAG_FIELDS \
    X(eva_string, "string") \
    X(eva_number, "number") \
    X(eva_bool, "bool") \
    X(eva_map, "map") \
    X(eva_list, "list") \
    X(eva_nil, "nil")

#define X(tag, ...) tag,
typedef enum { EVA_VALUE_TAG_FIELDS } EvaValueTag;
#undef X

typedef struct EvaValue {
    EvaValueTag tag;
    union {
        char *string;
        double number;
        int boolean;
    } data;
} EvaValue;

typedef struct EvaParser {
    size_t status;
    void *parser;
} EvaParser;

EvaValue (*original_get_value)(EvaParser*, char*, char*) = NULL;
EvaValue (*original_eva_get_list_field)(EvaValue, int) = NULL;
int (*original_get_list_length)(EvaValue) = NULL;
char (*original_check_exist_field_in_map)(EvaValue, char*) = NULL;
int (*original_get_map_length)(EvaValue) = NULL;
EvaValue (*original_eva_get_map_field)(EvaValue, char*) = NULL;
EvaValue (*original_eva_get_all_keys_from_map)(EvaValue) = NULL;

int initialized = 0;
void eva_wrapper_load_library(char* path) {
    if( initialized ) return;
    initialized = 1;

    void* handle = dlopen(path, RTLD_LAZY);
    if(! handle ) return;

    original_get_value = (EvaValue (*)(EvaParser*, char*, char*)) dlsym(handle, "eva_get_value_from_namespace");

    original_get_list_length = (int (*)(EvaValue)) dlsym(handle, "eva_get_list_length");
    original_eva_get_list_field = (EvaValue (*)(EvaValue, int)) dlsym(handle, "eva_get_list_field");

    original_check_exist_field_in_map = (char (*)(EvaValue, char*)) dlsym(handle, "eva_check_exist_field_in_map");
    original_get_map_length = (int (*)(EvaValue)) dlsym(handle, "eva_get_map_length");
    original_eva_get_map_field = (EvaValue (*)(EvaValue, char*)) dlsym(handle, "eva_get_map_field");
    original_eva_get_all_keys_from_map = (EvaValue (*)(EvaValue)) dlsym(handle, "eva_get_all_keys_from_map");
}

EvaValue* eva_get_value_from_namespace_wrapper(EvaParser *parser, char *ns, char *field) {
    EvaValue *data = malloc(sizeof(EvaValue));
    *data = original_get_value(parser, ns, field);
    return data;
}

int eva_get_list_length_wrapper(EvaValue *list) {
    if( list->tag != eva_list ) return 0;
    return original_get_list_length(*list);
}

EvaValue* eva_get_list_field_wrapper(EvaValue *list, int index) {
    EvaValue *data = malloc(sizeof(EvaValue));
    *data = original_eva_get_list_field(*list, index);
    return data;
}

char eva_check_exist_field_in_map_wrapper(EvaValue *map, char *index) {
    if( map->tag != eva_map ) return 0;
    return original_check_exist_field_in_map(*map, index);
}

int eva_get_map_length_wrapper(EvaValue *map) {
    if( map->tag != eva_map ) return 0;
    return original_get_map_length(*map);
}

EvaValue* eva_get_map_field_wrapper(EvaValue *map, char *index) {
    EvaValue *data = malloc(sizeof(EvaValue));
    *data = original_eva_get_map_field(*map, index);
    return data;
}

EvaValue* eva_get_all_keys_from_map_wrapper(EvaValue *map) {
    if( map->tag != eva_map ) return NULL;
    EvaValue *data = malloc(sizeof(EvaValue));
    *data = original_eva_get_all_keys_from_map(*map);
    return data;
}
