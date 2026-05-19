# EVA Declarative Configuration Language (eva-dcl)

EVA is a declarative, readable, and expressive configuration language designed to make application configuration simple and powerful. With EVA, you can compose data, use utility functions, and resolve values dynamically—all in a straightforward format.

## What is EVA?

EVA lets you:
- Define configurations in a clear and organized way
- Use utility functions like `env()`, `format()`, `merge()`, and more
- Compose and reuse values
- Make all your project smart, even also the configuration file

Example EVA configuration:

```eva
@project
name: "EVA"
version: "1.0.0"

@dev
name: "Lucas Silveira"
messages: [
    format("Hello, I am {}!", ref(name)),
    "Hello, world"
]
```

## Installation

With [Bun](https://bun.sh):

```sh-session
$ bun install eva-dcl
```

## Usage

```typescript
import { Eva, EvaMap, EvaList } from "eva-dcl";
import { join } from "path";

const config = new Eva(join(__dirname, "config.eva"));
const project_name = await config.get<string>("project", "name");
console.log(`Running ${project_name} project`);

const dev = await config.get<EvaMap>("dev", "name");
console.log(`created by ${dev}`);

const dev_messages = await config.get<EvaList>("dev", "messages");
const index = 0;
const message = await dev_messages.get(index);
console.log(`${dev} said: ${message}`);
```

Example `config.eva` file:

```eva
@project
name: "EVA"
version: "1.0.0"

@dev
name: "Lucas Silveira"
messages: [
    format("Hello, I am {}!", ref(name)),
    "Hello, world"
]
```

## Available Functions

You can use utility functions like:

| Function | Description | Example |
|---|---|---|
| `absolute()` | Returns the absolute version of a path | `absolute("./config")` |
| `basename()` | Returns the file name from a path | `basename("/home/file.txt")` |
| `clamp()` | Restricts a number between a minimum and maximum value | `clamp(volume, 0, 100)` |
| `coalesce()` | Returns the first non-null value | `coalesce(a, b, c)` |
| `contains()` | Checks whether a string, array or map contains a value | `contains(items, "apple")` |
| `debug()` | Prints or exposes internal debug information | `debug(config)` |
| `deepmerge()` | Recursively merges maps | `deepmerge(base, user)` |
| `else()` | Returns a fallback value if the first value is null | `else(port, 8080)` |
| `endswith()` | Checks whether a string ends with a value | `endswith(name, ".png")` |
| `entries()` | Returns all key-value pairs from a map | `entries(user)` |
| `env()` | Reads environment variables | `env("HOME")` |
| `extname()` | Returns the extension of a file | `extname("image.png")` |
| `format()` | Formats a string using placeholders | `format("Hello {}", name)` |
| `if()` | Returns one of two values based on a condition | `if(debug, "yes", "no")` |
| `important()` | Marks a value or section as important | `important(token)` |
| `keys()` | Returns all keys from a map | `keys(config)` |
| `lower()` | Converts text to lowercase | `lower(name)` |
| `merge()` | Merges maps shallowly | `merge(a, b)` |
| `ref()` | References local values inside the current namespace | `ref(home)` |
| `startswith()` | Checks whether a string starts with a value | `startswith(path, "/home")` |
| `trim()` | Removes leading and trailing whitespace | `trim(input)` |
| `upper()` | Converts text to uppercase | `upper(name)` |
| `values()` | Returns all values from a map | `values(config)` |
