import { read, CString } from "bun:ffi";
import type { Pointer } from "bun:ffi";
import { ValueType } from './enum'
import { EvaTypes } from "./main";
import { EvaNil } from './nil'
import { EvaList } from './list'
import { EvaMap } from './map'

const FIRST_INDEX = 0 as const;

function make_value_from<T extends EvaTypes>(ptr: Pointer, resolve: (value: T) => void) {
    const tag = read.u32(ptr, 0);

    switch(tag) {
        case ValueType.EVA_BOOL: resolve((read.u8(ptr, 8) ? true : false) as T);
        case ValueType.EVA_NUMBER: resolve(read.f64(ptr, 8) as T);
        case ValueType.EVA_NIL: resolve(new EvaNil() as T);

        case ValueType.EVA_STRING: {
            const value = read.ptr(ptr, 8);
            const char_ptr = new CString(value as Pointer, FIRST_INDEX);
            resolve(char_ptr.toString() as T);
        }

        case ValueType.EVA_LIST: {
            const list = new EvaList(ptr);
            resolve(list as T);
        }

        case ValueType.EVA_MAP: {
            const map = new EvaMap(ptr);
            resolve(map as T);
        }
    }
}

export { make_value_from };
