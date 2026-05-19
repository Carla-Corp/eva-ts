import { library, wrapper_utils } from './ffi'
import type { Pointer } from 'bun:ffi'
import { EvaNil, EvaTypes } from './main';
import { make_value_from } from './values';

class EvaList {
    private __pointer: Pointer;
    public length: number;
    constructor(ptr: Pointer) {
        this.__pointer = ptr;
        this.length = wrapper_utils.symbols.eva_get_list_length_wrapper(ptr);
    }

    list(): Promise<Array<EvaTypes>> {
        const promise = async (
            resolve: (value: Array<EvaTypes>) => void,
            reject: (reason: any) => void
        ) =>
        {   const arr: Array<EvaTypes> = [];
            for( let i = 0; i < this.length; i++)
            /* -> */ arr.push(await this.get(i));
            return resolve(arr);
        };

        return new Promise(promise);
    }

    get(i: number): Promise<EvaTypes> {
        if( i < 0 || i >= this.length ) return Promise.resolve(new EvaNil());

        const promise = (
            resolve: (value: EvaTypes) => void,
            reject: (reason: any) => void
        ) =>
        {   const ptr = wrapper_utils.symbols.eva_get_list_field_wrapper(this.__pointer, i);
            if(! ptr ) return reject(new Error('Failed to get value from namespace'));
            make_value_from(ptr, resolve);
        };

        return new Promise(promise);
    }

    [Symbol.toPrimitive](hint: string): string | null {
        if( hint === 'string' ) return `[Eva list of length ${this.length}]`
        return null;
    }
}

export { EvaList }
