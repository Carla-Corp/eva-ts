class EvaNil {
    constructor() {}
    defaultValue(): null { return null; }
    toString(): string { return 'nil' }
    valueOf(): null { return null; }
    [Symbol.toPrimitive](hint: string): any {
        if( hint === 'string' ) return 'nil';
        if( hint === 'number' ) return 0;
        return null;
    }
}

export { EvaNil };
