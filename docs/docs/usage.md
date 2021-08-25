---
title: Usage
---

## General usage
In general, you call the cli like so: <br>
`ccom [options] <input>`

For example a call could look like this:
`ccom --data ./data-file.json --out-file ./output-file.yml ./input-file.yml`

For customization, you have following cli options:

| Option                                        | Shortcut         | Description                                                                           | Default |
|-----------------------------------------------|------------------|---------------------------------------------------------------------------------------|---------|
| `--benchmark <number>`                        | `-b <no>`        | Execute compiler benchmarks. n is the number of benchmark runs                        | 0       |
| `--compiler <name>` (temporarily unavailable) | `-c <name>`      | Can be used to switch the compiler backend. Valid inputs are `cpp` and `java`         | "cpp"   |
| `--data <data>`                               | `-d <data>`      | JSON string or path to JSON file, which holds the evaluation work data                | {}      |
| `--lang <lang>`                               | `-l <lang>`      | File format / programming language (e.g. `yaml`, `java`, `html`, ...)                 | "auto"* |
| `--mode-single`                               | `-m`             | Set input mode to single statement list                                               | -       |
| `--out-file <path>`                           | `-o <path>`      | Path to output file. If you omit this flag, the output will be printed to the console | -       |
| `--silent`                                    | `-s`             | Only print raw compiler output and no debug output                                    | -       |
| `--force`                                     | `-f`             | Ignore safety checks. Warning: This could cause demage                                | -       |
| `--line-comment-iden <string>`                | `-lci <string>`  | Specifies the line comment char(s) of your data format                                | "#"     |
| `--block-comment-iden-open <string>`          | `-bcio <string>` | Specifies the opening block comment char(s) of your data format                       | ""      |
| `--block-comment-iden-close <string>`         | `-bcic <string>` | Specifies the closing block comment char(s) of your data format                       | ""      |

## File mode vs single mode
CCom supports two modes of usage: <br>
- The input file / string with conditional sections to an output file / string
- The single mode, which can evaluate a single statement list to a boolean

Here is an example for an input file, in this case a YAML file: <br>
```yaml
build: ${{SPRING_MAVEN_SOURCE_DIRECTORY}}
restart: always
networks:
#? if service.frontend[1].preselected == false {
#  - frontend-backend
#? }
ports:
  - ${{SPRING_MAVEN_PORT}}:8080
env_file:
#? if service.frontend[0] contains name != "spring-maven" {
#   - environment.env
#? }
```

Here is an example for an input for a single statement list: <br>
`has this.is.a.test.key | this.is contains a.test.key == "value" | this.is.test >= 42`

The cli defaults the mode to the input file, however you can switch to single mode by using the `--mode-single` or `-m` flag for the cli.

## Data path vs data string
CCom accepts data in the JSON format only, but you can give it to the cli in one of two different forms: <br>
- Data file: If you have the json input data in form of a file, you can set `--data` / `-d` to the relative or absolute path to the data file. CCom will then load the file from this path.
- Data string: If you do not have the data in form of a file, you might want CCom to consume a string. For that you can set `--data` / `-d` to a JSON string (e.g.: `{"test-attribute": "test-value"}`)

## Language
As CCom does not know the content type of your input file, you sometimes have to specify the files language or the comment identifiers. CCom is smart enough to recognize the language for some file extensions, but if the file contents does not match the extension, you can set the language manually via`--lang` / `-l`. This should work out for following languages:

| Language | Language   | Language | Language   | Language           |
| -------- | ---------- | -------- | ---------- | ------------------ |
| Assembly | C          | C++      | Dart       | Dockerfile         |
| Elexir   | Go         | Groovy   | Haskell    | HTML               |
| Java     | JavaScript | Julia    | Kotlin     | Lua                |
| Pascal   | Perl       | PHP      | Powershell | Python             |
| R        | Ruby       | Rust     | Spice      | SQL                |
| Swift    | TypeScript | XML      | YAML       | *more to come ...* |

If you did not find your language in the list, you can set the comment identifiers by hand. Here is an example: <br>
`ccom --data ./test-data.json --line-comment-iden "//" --block-comment-iden-open "/*" --block-comment-iden-close "*/" ./test-file.java`

## Switch compilers (temporarily unavailable)
CCom has multiple compiler frontend implementations, which have different strengths and weaknesses. The default compiler backend is called `cpp`.
To switch between compiler implementations, you can use `--compiler` / `-c`. Valid values are `cpp` and `java`.

## Benchmarking
CCom comes with a built-in benchmark mode to test the performance of compiling different files in combination with different compiler backends. To run a benchmark for an input file with input data, you can use `--benchmark` / `-b` and specify a number of runs. <br>
E.g.: `ccom  --data ./test-data.json --benchmark 500 --out-file ./output.yml ./input-file.yml`

CCom will show you how long every run takes as well as the total and average runtime.

## Silent mode
By default, CCom prints some information about the compile status to the console. If you're using CCom for your purpose, you might not want such compile status outputs. To disable all CCom outputs, except the compile result output, you can use the `--silent` or `-s` flag for the cli.

## Force flag
CCom warns you per default if the output file already exists. To disable that warning, you can simply use the `--force` or `-f` flag for the cli.