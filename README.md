|:warning: | This language is under construction and doesn't work in the current state. Do NOT use it in production. |
|----------|:-------------------------------|

# CCom (Conditional Comments) language

*Note: This language is part of the [Compose Generator](https://github.com/compose-generator/compose-generator) project, but also can be used independently.*

## Introduction
CCom is a language for pre-processing source files. It's primary purpose is to evaluate conditional sections in formats like YAML or XML, but can also be used for a variety of programming languages with support for comments.

### Supported data formats
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

### Input
CCom requests two input parameters. A source file/string of any language, containing conditional sections and a JSON file/string, which holds data for the evaluation.

#### Source file
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

Another example for Java (whatever you want to achive with that)
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

Java example:
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

#### Data input
```json
{
    "version": "0.7.0",
    "service": {
        "frontend": [
            {
                "label": "Spring Maven",
                "name": "spring-maven",
                "dir": "./spring-maven",
                "type": "frontend",
                "preselected": false
            },
            {
                "label": "Spring Gradle",
                "name": "spring-gradle",
                "dir": "./spring-gradle",
                "type": "frontend",
                "preselected": false
            }
        ],
        "backend": []
    },
    "var": [
        {
            "name": "SPRING_MAVEN_SOURCE_DIRECTORY",
            "value": "./backend"
        },
        {
            "name": "SPRING_MAVEN_PACKAGE_NAME",
            "value": "com.chillibits.test-app"
        }
    ]
}
```

#### CLI flags
-   `--line-comment-char="//"`
-   `--block-comment-char-open="/*"`
-   `--block-comment-char-close="*/"`
-	`--data="{}"`
-   `--force`
-	`--mode="single" / --mode="file"`
-   `--out-file="./output.java"`
-   `--preserve-comments-on-false`

### Single condition + data input => boolean output
You also have the option to feed CCom with a single condition and get a boolean output, whether this condition is true or false.
```ccom
has service.angular | has service.mysql | has backend
```

In combination with the data input from above, this condition would be `false`, because the data collection does not fulfill any of the three sub-conditions.


## Grammar
*Note a grammar is dependent on the line comment chars and the block comment chars. In this particular case the line comment char is `//`, the block comment char open is `/*` and the block comment char close is `*/`*

Start symbol: `CONTENT`.

```
CONTENT               --> (CHARS SECTION)* CHARS
SECTION               --> (COM_LINE_BLOCK* COM_BLOCK_BLOCK*)*
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
KEY                   --> IDENTIFIER(.IDENTIFIER)*
IDENTIFIER            --> LETTER+
VALUE                 --> STRING | NUMBER
STRING                --> "CHARS_LIMITED"
NUMBER                --> DIGIT+
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

## Work process
The first thing CCom does, is to analyze the input and determine, whether it is a single condition or a source file. Depending on that, the CLI interface will call the main compiler with `--mode="single"` or `--mode="file"`.

### Source file
CCom takes the whole file and feeds it into the interpreter, starting with the `CONTENT` grammar node.

### Single condition
CCom feeds the condition into the interpreter, starting by the `STMT_LST` grammar node.

## Usage
*To be extended...*

## Contribute otherwise to the project
If you want to contribute to this project, please ensure you comply with the contribution guidelines.

© Marc Auberer 2021
