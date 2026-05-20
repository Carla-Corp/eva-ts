import { resolve } from 'path'
import { platform, arch } from 'os'
import { cc, dlopen, FFIType, suffix } from 'bun:ffi'

const library_path = resolve(__dirname, '../eva/libeva-' + platform() + '-' + arch() + '.' + suffix);
const win_wrapper_path = resolve(__dirname, '../eva/libwrapper-' + arch() + '.' + suffix);

const library = dlopen(library_path, {
    'eva_make_parser': {
        returns: FFIType.pointer,
        args: [FFIType.cstring],
    },
    'eva_check_exist_field_in_namespace': {
        returns: FFIType.bool,
        args: [FFIType.pointer, FFIType.cstring, FFIType.cstring],
    },
});

let wrapper_utils;

const wrapper_symbols = {
    'eva_wrapper_load_library': {
        returns: FFIType.void,
        args: [FFIType.cstring],
    },
    'eva_get_value_from_namespace_wrapper': {
        returns: FFIType.pointer,
        args: [FFIType.pointer, FFIType.cstring, FFIType.cstring],
    },
    'eva_get_list_length_wrapper': {
        returns: FFIType.i32,
        args: [FFIType.pointer],
    },
    'eva_get_list_field_wrapper': {
        returns: FFIType.pointer,
        args: [FFIType.pointer, FFIType.i32],
    },
    'eva_check_exist_field_in_map_wrapper': {
        returns: FFIType.bool,
        args: [FFIType.pointer, FFIType.cstring],
    },
    'eva_get_map_field_wrapper': {
        returns: FFIType.pointer,
        args: [FFIType.pointer, FFIType.cstring],
    },
    'eva_get_map_length_wrapper': {
        returns: FFIType.i32,
        args: [FFIType.pointer],
    },
    'eva_get_all_keys_from_map_wrapper': {
        returns: FFIType.pointer,
        args: [FFIType.pointer],
    },
};

if( platform() != 'win32' ) wrapper_utils = cc({
    source: resolve(__dirname, '../src/wrapper.c'),
    symbols: wrapper_symbols
});

else wrapper_utils = opendl(win_wrapper_path, wrapper_symbols)
export { library_path, library, wrapper_utils };
