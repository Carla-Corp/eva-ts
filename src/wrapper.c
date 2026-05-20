/*
    Cross-platform EVA wrapper
    Windows + Linux/macOS
*/

#ifdef _WIN32

    #include <windows.h>

    #define DYNLIB_HANDLE HMODULE
    #define LOAD_LIBRARY(path) LoadLibraryA(path)
    #define LOAD_SYMBOL(handle, name) GetProcAddress(handle, name)

#else

    #include <dlfcn.h>

    #define DYNLIB_HANDLE void*
    #define LOAD_LIBRARY(path) dlopen(path, RTLD_LAZY)
    #define LOAD_SYMBOL(handle, name) dlsym(handle, name)

#endif

#include <stdbool.h>
#include <stdlib.h>

/* =========================
   EVA TYPES
   ========================= */

#define EVA_VALUE_TAG_FIELDS \
    X(eva_string, "string") \
    X(eva_number, "number") \
    X(eva_bool, "bool") \
    X(eva_map, "map") \
    X(eva_list, "list") \
    X(eva_nil, "nil")

#define X(tag, ...) tag,
typedef enum {
    EVA_VALUE_TAG_FIELDS
} EvaValueTag;
#undef X

typedef struct EvaValue {
    EvaValueTag tag;

    union {
        char* string;
        double number;
        int boolean;
    } data;
} EvaValue;

typedef struct EvaParser {
    size_t status;
    void* parser;
} EvaParser;

/* =========================
   ORIGINAL FUNCTIONS
   ========================= */

EvaValue (*original_get_value)(
    EvaParser*,
    char*,
    char*
) = NULL;

EvaValue (*original_eva_get_list_field)(
    EvaValue,
    int
) = NULL;

int (*original_get_list_length)(
    EvaValue
) = NULL;

char (*original_check_exist_field_in_map)(
    EvaValue,
    char*
) = NULL;

int (*original_get_map_length)(
    EvaValue
) = NULL;

EvaValue (*original_eva_get_map_field)(
    EvaValue,
    char*
) = NULL;

EvaValue (*original_eva_get_all_keys_from_map)(
    EvaValue
) = NULL;

/* =========================
   INIT
   ========================= */

int initialized = 0;

void eva_wrapper_load_library(char* path) {
    if (initialized) {
        return;
    }

    initialized = 1;

    DYNLIB_HANDLE handle = LOAD_LIBRARY(path);

    if (!handle) {
        return;
    }

    original_get_value =
        (EvaValue (*)(EvaParser*, char*, char*))
        LOAD_SYMBOL(handle, "eva_get_value_from_namespace");

    original_get_list_length =
        (int (*)(EvaValue))
        LOAD_SYMBOL(handle, "eva_get_list_length");

    original_eva_get_list_field =
        (EvaValue (*)(EvaValue, int))
        LOAD_SYMBOL(handle, "eva_get_list_field");

    original_check_exist_field_in_map =
        (char (*)(EvaValue, char*))
        LOAD_SYMBOL(handle, "eva_check_exist_field_in_map");

    original_get_map_length =
        (int (*)(EvaValue))
        LOAD_SYMBOL(handle, "eva_get_map_length");

    original_eva_get_map_field =
        (EvaValue (*)(EvaValue, char*))
        LOAD_SYMBOL(handle, "eva_get_map_field");

    original_eva_get_all_keys_from_map =
        (EvaValue (*)(EvaValue))
        LOAD_SYMBOL(handle, "eva_get_all_keys_from_map");
}

/* =========================
   WRAPPERS
   ========================= */

EvaValue* eva_get_value_from_namespace_wrapper(
    EvaParser* parser,
    char* ns,
    char* field
) {
    if (!original_get_value) {
        return NULL;
    }

    EvaValue* data = (EvaValue*) malloc(sizeof(EvaValue));

    if (!data) {
        return NULL;
    }

    *data = original_get_value(parser, ns, field);

    return data;
}

int eva_get_list_length_wrapper(EvaValue* list) {
    if (!list) {
        return 0;
    }

    if (list->tag != eva_list) {
        return 0;
    }

    if (!original_get_list_length) {
        return 0;
    }

    return original_get_list_length(*list);
}

EvaValue* eva_get_list_field_wrapper(
    EvaValue* list,
    int index
) {
    if (!list) {
        return NULL;
    }

    if (list->tag != eva_list) {
        return NULL;
    }

    if (!original_eva_get_list_field) {
        return NULL;
    }

    EvaValue* data = (EvaValue*) malloc(sizeof(EvaValue));

    if (!data) {
        return NULL;
    }

    *data = original_eva_get_list_field(*list, index);

    return data;
}

char eva_check_exist_field_in_map_wrapper(
    EvaValue* map,
    char* index
) {
    if (!map) {
        return 0;
    }

    if (map->tag != eva_map) {
        return 0;
    }

    if (!original_check_exist_field_in_map) {
        return 0;
    }

    return original_check_exist_field_in_map(*map, index);
}

int eva_get_map_length_wrapper(EvaValue* map) {
    if (!map) {
        return 0;
    }

    if (map->tag != eva_map) {
        return 0;
    }

    if (!original_get_map_length) {
        return 0;
    }

    return original_get_map_length(*map);
}

EvaValue* eva_get_map_field_wrapper(
    EvaValue* map,
    char* index
) {
    if (!map) {
        return NULL;
    }

    if (map->tag != eva_map) {
        return NULL;
    }

    if (!original_eva_get_map_field) {
        return NULL;
    }

    EvaValue* data = (EvaValue*) malloc(sizeof(EvaValue));

    if (!data) {
        return NULL;
    }

    *data = original_eva_get_map_field(*map, index);

    return data;
}

EvaValue* eva_get_all_keys_from_map_wrapper(
    EvaValue* map
) {
    if (!map) {
        return NULL;
    }

    if (map->tag != eva_map) {
        return NULL;
    }

    if (!original_eva_get_all_keys_from_map) {
        return NULL;
    }

    EvaValue* data = (EvaValue*) malloc(sizeof(EvaValue));

    if (!data) {
        return NULL;
    }

    *data = original_eva_get_all_keys_from_map(*map);

    return data;
}
