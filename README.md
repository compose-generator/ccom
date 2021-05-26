# CCom (Conditional Comments) language

|:warning: | This language is under construction and doesn't work in its current state. Do NOT use it in production. |
|----------|:-------------------------------|

*Note: This language is part of the [Compose Generator](https://github.com/compose-generator/compose-generator) project, but also can be used independently.*

## Introduction
CCom is a language for pre-processing source files. It's primary purpose is to evaluate conditional sections in formats like YAML or XML, but can also be used for a variety of programming languages with support for comments.

## Supported data formats
|            | Line comment chars | Block comment chars open | Block comment chars close |
|------------|--------------------|--------------------------|---------------------------|
| YAML       | #                  | -                        | -                         |
| Java       | //                 | /*                       | */                        |
| Go         | //                 | /*                       | */                        |
| C          | //                 | /*                       | */                        |
| C++        | //                 | /*                       | */                        |
| JavaScript | //                 | /*                       | */                        |
| TypeScript | //                 | /*                       | */                        |
| Dockerfile | #                  | -                        | -                         |
| HTML       | -                  | <!--                     | -->                       |
| XML        | -                  | <!--                     | -->                       |
| Rust       | //                 | /*                       | */                        |
| Assembler  | ;                  | -                        | -                         |

*Note: Formats like JSON, where no comments are supported can also work with CCom, however then the file input is not valid before pre-processing it with CCom.*

## Usage
In general, you call the cli like so: <br>
`ccom [options] <input>`

### CLI options
| Option                      	| Shortcut 	| Description                                                                           	| Default 	|
|-----------------------------	|----------	|---------------------------------------------------------------------------------------	|---------	|
| --benchmark                  	| -b <n>       	| Execute compiler benchmarks. <n> is the number of benchmark runs	                	| 0      	|
| --compiler                  	| -c <name>    	| Can be used to switch the compiler backend. Valid inputs are `cpp` and `java`	               	| cpp      	|
| --data                      	| -d       	| JSON string or path to JSON file, which holds the evaluation work data                	| {}      	|
| --lang                      	| -l       	| File format / programming language (e.g. yaml, java, html, ...)                       	| yaml    	|
| --mode-single               	| -m       	| Set input mode to single statement list                                               	| -       	|
| --out-file                  	| -o       	| Path to output file. If you omit this flag, the output will be printed to the console 	| -       	|
| --silent                    	| -s       	| Only print raw compiler output and no debug output                                    	| -       	|
| --force                     	| -f       	| Ignore safety checks. Warning: This could cause demage                                	| -       	|
| --line-comment-char         	| -lcc     	| Specifies the line comment char(s) of your data format                                	| #       	|
| --block-comment-chars-open  	| -bcco    	| Specifies the opening block comment char(s) of your data format                       	| -       	|
| --block-comment-chars-close 	| -bccc    	| Specifies the closing block comment char(s) of your data format                       	| -       	|

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

Another example for Java with line comments (whatever you want to achive with that)
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
*Note a grammar is dependent on the line comment chars and the block comment chars. In this particular case the line comment char is `//`, the block comment char open is `/*` and the block comment char close is `*/`*

Start symbol: `CONTENT`.

```
CONTENT               --> (CHARS SECTION)* CHARS
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
STMT                  --> HAS_STMT | COMP_STMT
HAS_STMT              --> has KEY | not has KEY
COMP_STMT             --> KEY == VALUE | KEY != VALUE
KEY                   --> IDENTIFIER INDEX? (.IDENTIFIER INDEX?)*
INDEX                 --> [NUMBER]
IDENTIFIER            --> LETTER+
VALUE                 --> STRING | NUMBER | BOOLEAN
STRING                --> "CHARS_LIMITED"
NUMBER                --> DIGIT+
BOOLEAN               --> true | false
CHARS                 --> ({UNICODE}\{COM_LINE_IDEN, COM_BLOCK_IDEN_OPEN})*
CHARS_LIMITED         --> (LETTER* DIGIT* SCHAR*)*
LETTER                --> a|b|...|y|z|A|B|...|Y|Z
DIGIT                 --> 0|1|2|3|4|5|6|7|8|9
SCHAR                 --> -|.|_|[|]|{|}|/|\|'
UNICODE               --> Any unicode character
```

## Keywords
-   `if`
-   `has`
-   `not`

## Special characters
-   `?`
-   `|`
-   `.`
-   `"`
-   `{` / `}`

## AST Classes
![Class diagram](media/class-diagram.svg "Class diagram")

## Contribute otherwise to the project
If you want to contribute to this project, please ensure you comply with the contribution guidelines.

© Marc Auberer 2021
