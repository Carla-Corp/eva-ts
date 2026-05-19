import { errors } from './errors'
import { basename } from 'path'
import { arch } from 'os'
import { EvaList } from './list'
import { CString, read } from 'bun:ffi'
import type { Pointer } from 'bun:ffi'
import { library, library_path, wrapper_utils } from './ffi'
import { EvaNil } from './nil'
import { EvaMap } from './map'
import { make_value_from } from './values'

type EvaTypes = string | number | boolean | EvaNil | EvaList | EvaMap;

function system_bits(): number {
    return ( arch() == 'x64' || arch() == 'arm64' ) ? 64 : 32;
}

const cstr = (x: string) => Buffer.from(x + '\0');

class Eva {
    private path: string;
    private driver: Pointer | null;
    private filename: string;

    constructor(filepath: string) {
        this.path = filepath;
        this.filename = basename(filepath);
        this.driver = library.symbols.eva_make_parser(
            Buffer.from(this.path)
        );

        wrapper_utils.symbols.eva_wrapper_load_library(
            Buffer.from(library_path)
        );

        if(! this.driver ) throw new Error('Failed to spawn the Eva parser');
        const status = read.u32(this.driver, 0);

        if( status != 0 ) throw new Error(
            errors[status](this) ?? 'Unknown error'
        );

    }

    get<T extends EvaTypes>(namespace: string, field: string): Promise<T> {
        const exists = library.symbols.eva_check_exist_field_in_namespace(
            this.driver,
            cstr(namespace),
            cstr(field)
        );

        if(! exists ) return Promise.reject(
            new Error(`Field ${field} does not exist in namespace ${namespace}`)
        );

        const promise = (
            resolve: (value: T) => void,
            reject: (reason: any) => void
        ): void => {
            const result = wrapper_utils.symbols.eva_get_value_from_namespace_wrapper(
                this.driver,
                cstr(namespace),
                cstr(field)
            );

            if(! result ) return reject(new Error('Failed to get value from namespace'));
            make_value_from(result, resolve);
        };

        return new Promise(promise);
    }

    // [Symbol.dispose]() {
    //     if( this.driver ) {
    //         wrapper_utils.symbols.eva_wrapper_dump_driver(this.driver)
    //     }
    // }
}

export { Eva, EvaNil, EvaTypes, EvaList, EvaMap };
