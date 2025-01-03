<p align="center">
  <img alt="CCom Logo" src="./media/logo-wide.png" height="280" />
  <h3 align="center">CCom language</h3>
  <p align="center">Compiler for the CCom (Conditional Comments) language.</p>
  <p align="center">
    <a target="_blank" href="https://github.com/compose-generator/ccom/releases/latest"><img src="https://img.shields.io/github/v/release/compose-generator/ccom?include_prereleases"></a>
    <a target="_blank" href="https://hub.docker.com/r/chillibits/ccom"><img src="https://img.shields.io/docker/pulls/chillibits/ccom"></a>
    <a target="_blank" href="https://github.com/compose-generator/ccom/actions/workflows/ci-go.yml"><img src="https://github.com/compose-generator/ccom/actions/workflows/ci-go.yml/badge.svg"></a>
	<a target="_blank" href="https://github.com/compose-generator/ccom/actions/workflows/ci-cpp.yml"><img src="https://github.com/compose-generator/ccom/actions/workflows/ci-cpp.yml/badge.svg"></a>
	<a target="_blank" href="https://github.com/compose-generator/ccom/actions/workflows/ci-java.yml"><img src="https://github.com/compose-generator/ccom/actions/workflows/ci-java.yml/badge.svg"></a>    
	<a target="_blank" href="https://github.com/compose-generator/ccom/actions/workflows/codeql-analysis.yml"><img src="https://github.com/compose-generator/ccom/actions/workflows/codeql-analysis.yml/badge.svg"></a>
    <a target="_blank" href="https://goreportcard.com/report/github.com/compose-generator/ccom"><img src="https://goreportcard.com/badge/github.com/compose-generator/ccom"></a>
    <a target="_blank" href="https://makeapullrequest.com"><img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg"></a>
    <a target="_blank" href="./LICENSE"><img src="https://img.shields.io/github/license/compose-generator/ccom"></a>
  </p>
</p>

*Note: This language is part of the [Compose Generator](https://github.com/compose-generator/compose-generator) project, but also can be used independently.*

## Introduction
CCom is a language for pre-processing source files. It's primary purpose is to evaluate conditional sections in formats like YAML or XML, but can also be used for a variety of programming languages with support for comments.

## Documentation
Please visit the documentation at [ccom.compose-generator.com](https://ccom.compose-generator.com).

## Supported data formats
|            | Line comment iden | Block comment iden open | Block comment iden close |
|------------|-------------------|-------------------------|--------------------------|
| Assembly   | ;                 | -                       | -                        |
| C          | //                | /*                      | */                       |
| C++        | //                | /*                      | */                       |
| Dart       | //                | /*                      | */                       |
| Dockerfile | #                 | -                       | -                        |
| Elixir     | #                 | """                     | """                      |
| Go         | //                | /*                      | */                       |
| Groovy     | //                | /*                      | */                       |
| Haskell    | --                | -{                      | -}                       |
| HTML       | -                 | <!--                    | -->                      |
| Java       | //                | /*                      | */                       |
| JavaScript | //                | /*                      | */                       |
| Julia      | #                 | #=                      | =#                       |
| Kotlin     | //                | /*                      | */                       |
| Lua        | --                | --[[                    | ]]                       |
| Pascal     | -                 | (*                      | *)                       |
| Perl       | #                 | =item                   | =cut                     |
| PHP        | //                | /*                      | */                       |
| Powershell | #                 | <#                      | #>                       |
| Python     | #                 | """                     | """                      |
| R          | #                 | -                       | -                        |
| Ruby       | #                 | =begin                  | =end                     |
| Rust       | //                | /*                      | */                       |
| Spice      | //                | /*                      | */                       |
| SQL        | --                | -                       | -                        |
| Swift      | //                | /*                      | */                       |
| TypeScript | //                | /*                      | */                       |
| XML        | -                 | <!--                    | -->                      |
| YAML       | #                 | -                       | -                        |

*Note: Formats like JSON, where no comments are supported can also work with CCom, however then the file input is not valid before pre-processing it with CCom.*

## Install
<details><summary><b>Install on Debian / Ubuntu</b></summary>
<p>

```sh
$ sudo apt-get install ca-certificates
$ curl -fsSL https://server.chillibits.com/files/repo/gpg | sudo apt-key add -
$ sudo add-apt-repository "deb https://repo.chillibits.com/$(lsb_release -is | awk '{print tolower($0)}')-$(lsb_release -cs) $(lsb_release -cs) main"
$ sudo apt-get update
$ sudo apt-get install ccom
```

</p>
</details>

<details><summary><b>Install on Fedora</b></summary>
<p>

```sh
$ sudo dnf -y install dnf-plugins-core
$ sudo dnf config-manager --add-repo https://server.chillibits.com/files/repo/fedora.repo
$ sudo dnf install ccom
```

</p>
</details>

<details><summary><b>Install on CentOS</b></summary>
<p>

```sh
$ sudo yum install -y yum-utils
$ sudo yum-config-manager --add-repo https://server.chillibits.com/files/repo/centos.repo
$ sudo yum install ccom
```

</p>
</details>

<details><summary><b>Install on Raspbian</b></summary>
<p>

```sh
$ sudo apt-get install ca-certificates
$ curl -fsSL https://server.chillibits.com/files/repo/gpg | sudo apt-key add -
$ sudo echo "deb [arch=armhf] https://repo.chillibits.com/$(lsb_release -is | awk '{print tolower($0)}')-$(lsb_release -cs) $(lsb_release -cs) main" > /etc/apt/sources.list.d/chillibits.list
$ sudo apt-get update
$ sudo apt-get install ccom
```

</p>
</details>

<details><summary><b>Install on Windows</b></summary>
<p>

CCom gets distributed for Windows via the new Windows package manager called [winget](https://github.com/microsoft/winget-cli). In the future, winget will be available for download in the Microsoft Store. Currently, the easiest way to install winget is, to download it manually from GitHub. Visit the [installation instruction](https://github.com/microsoft/winget-cli#installing-the-client) from Microsoft. <br>
As soon as the Windows package manager is installed on your Windows machine, you can open powershell and execute this installation command: <br>
```sh
$ winget install ChilliBits.CCom
```
    
</p>
</details>

<details><summary><b>Use with Docker</b></summary>
<p>

**Linux:**
```sh
$ docker run --rm -it -v $(pwd):/ccom/out chillibits/ccom
```

**Windows:**
```sh
$ docker run --rm -it -v ${pwd}:/ccom/out chillibits/ccom
```
*Note: This command does not work with Windows CMD command line. Please use Windows PowerShell instead.*

</p>
</details>

## Usage
In general, you call the cli like so: <br>
`ccom [options] <input>`

### CLI options
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

*) Lang "auto" determines the language based on the file extension of the input file.

## Work process
The first thing CCom does, is to analyze the input and determine, whether it is a single condition or a source file. This depends on how you call the CCom CLI. To switch to a single statement, you can call it with the flag `--mode-single`

### Source file mode
CCom takes the whole file and feeds it into the interpreter, starting with the `CONTENT` grammar node.

Here is an example YAML file, which can be evaluated by CCom:
```yaml
...
property1: "value1"
#? if has service.angular | has service.mysql {
# property2:
# 	property3: true
#? }
...
```

Another example for Java with line comments (whatever you want to achieve with that)
```java
public class Example {
	public static void main(String[] args) {
		//? if has property_name | has property
		//? {
		// if () {
		//    System.out.println("True");
		// }
		//? }
		//? if not has property_name {
		// System.out.println("False");
		//? }
	}
}
```

Another Java example with block comments:
```java
public class Example {
	public static void main(String[] args) {
		/*? if has property_name {
			if () {
				System.out.println("True");
			}
		}*/
		/*? if has property_name {
			if () {
				System.out.println("True");
			}
		}*/
	}
}
```

### Single condition mode (`--mode-single`)
CCom feeds the condition into the interpreter, starting by the `STMT_LST` grammar node. The result is either `true` or `false`.

Here is an example input: <br>
```ccom
has service.angular | has service.frontend[1] | has backend
```

### Data file
CCom needs a JSON data tree to work with. To pass a file or a JSON string to CCOM, please use the `--data` flag. <br>
An example file looks like this:
```json
{
    "version": "0.7.0",
    "service": {
        "frontend": [
            {
                "label": "Spring Maven",
                "name": "spring-maven",
                "dir": "./spring-maven"
            },
            {
                "label": "Spring Gradle",
                "preselected": false
            }
        ],
        "backend": []
    },
    "var": [
        {
            "name": "SPRING_MAVEN_SOURCE_DIRECTORY"
        },
        {
            "name": "SPRING_MAVEN_PACKAGE_NAME",
            "value": "com.chillibits.test-app"
        }
    ]
}
```
To access `0.7.0`, you can use the key `version`. To access `./spring-maven`, you can use the key `service.frontend[0].dir`.


## Grammar
*Note a grammar is dependent on the line comment identifiers and the block comment identifiers. In this particular case the line comment char is `//`, the block comment char open is `/*` and the block comment char close is `*/`*

Start symbol: `CONTENT`.

```
CONTENT               --> CHARS (SECTION CHARS)*
SECTION               --> COM_LINE_BLOCK | COM_BLOCK_BLOCK
COM_LINE_BLOCK        --> COM_LINE_IDEN if STMT_LST COM_LINE_IDEN? { PAYLOAD COM_LINE_IDEN }
COM_BLOCK_BLOCK       --> COM_BLOCK_IDEN_OPEN IF_BLOCK COM_BLOCK_IDEN_CLOSE
IF_BLOCK              --> if STMT_LST { PAYLOAD }
COM_LINE_IDEN         --> //?
COM_IDEN_PAYLOAD      --> //
COM_BLOCK_IDEN_OPEN   --> /*?
COM_BLOCK_IDEN_CLOSE  --> */
PAYLOAD               --> (COM_IDEN_PAYLOAD CHARS)+
STMT_LST              --> STMT (`|` STMT)*
STMT                  --> HAS_STMT | COMP_STMT | CONTAINS_STMT
HAS_STMT              --> not? has KEY
COMP_STMT             --> KEY COMP_OP VALUE
CONTAINS_STMT         --> KEY not? contains KEY COMP_OP VALUE
KEY                   --> IDENTIFIER INDEX? (.IDENTIFIER INDEX?)*
INDEX                 --> [NUMBER]
IDENTIFIER            --> (Letter|UNDERSCORE) (LETTER* DIGIT* UNDERSCORE*)*
VALUE                 --> STRING | NUMBER | BOOLEAN
STRING                --> "CHARS_LIMITED"
NUMBER                --> DIGIT+
BOOLEAN               --> true | false
COMP_OP               --> == | != | < | > | <= | >=
CHARS                 --> ({UNICODE}\{COM_LINE_IDEN, COM_BLOCK_IDEN_OPEN})*
CHARS_LIMITED         --> (LETTER* DIGIT* SCHAR* UNDERSCORE*)*
LETTER                --> a|b|...|y|z|A|B|...|Y|Z
DIGIT                 --> 0|1|2|3|4|5|6|7|8|9
SCHAR                 --> -|.|[|]|{|}|/|\|'
UNDERSCORE            --> _
UNICODE               --> Any unicode character
```

## Keywords
-   `if`
-   `has`
-   `not`
-   `contains`

## Special characters
-   `?`
-   `|`
-   `.`
-   `"`
-   `{` / `}`

## AST Classes
![Class diagram](media/class-diagram.svg "Class diagram")

## Contribute to the project
If you want to contribute to this project, please ensure you comply with the [contribution guidelines](./CONTRIBUTING.md).

© Marc Auberer 2021-2023
