import { resolve } from 'path';
import { Eva, EvaList, EvaMap } from '../src/main';

const path = resolve(__dirname, './test.eva');
const eva = new Eva(path);

const author = await eva.get<EvaMap>('project', 'author');
const keys = await author.keys();
console.log(`Keys: ${await keys.list()}`)
