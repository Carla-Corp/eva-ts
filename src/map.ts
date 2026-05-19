import { library, wrapper_utils } from './ffi'
import type { Pointer } from 'bun:ffi'
import { EvaList, EvaNil, EvaTypes } from './main';
import { make_value_from } from './values';

const cstr = (x: string) => Buffer.from(x + '\0');

class EvaMap {
    private __pointer: Pointer;
    public length: number;
    constructor(ptr: Pointer) {
        this.__pointer = ptr;
        this.length = wrapper_utils.symbols.eva_get_map_length_wrapper(ptr);
    }

    keys(): Promise<EvaList> {
        const promise = async (
            resolve: (value: EvaList) => void,
            reject: (reason: any) => void
        ) =>
        {   const ptr = wrapper_utils.symbols.eva_get_all_keys_from_map_wrapper(this.__pointer);
            if(! ptr ) return reject(new Error('Failed to get keys from map'));
            return resolve(new EvaList(ptr));
        };

        return new Promise(promise);
    }

    get(i: string): Promise<EvaTypes> {
        if(! wrapper_utils.symbols.eva_check_exist_field_in_map_wrapper(this.__pointer, cstr(i)) )
            return Promise.resolve(new EvaNil());

        const promise = (
            resolve: (value: EvaTypes) => void,
            reject: (reason: any) => void
        ) =>
        {   const ptr = wrapper_utils.symbols.eva_get_map_field_wrapper(this.__pointer, cstr(i));
            if(! ptr ) return reject(new Error('Failed to get value from namespace'));
            make_value_from(ptr, resolve);
        };

        return new Promise(promise);
    }

    [Symbol.toPrimitive](hint: string): string | null {
        if( hint === 'string' ) return `[Eva map of length ${this.length}]`
        return null;
    }
}

export { EvaMap }
