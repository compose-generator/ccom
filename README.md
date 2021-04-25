# CCom (Conditional Comments) language

*Note: This language is part of the Compose Generator project, but also can be used independently.*

## Introduction
CCom is a language for pre-processing source files. It's primary purpose is to evaluate conditional sections in formats like YAML or JSON, but can also be used for a variety of programming languages with support for comments.

### Input
CCom requests two input parameters. A source file/string of any language, containing conditional sections and a JSON file/string, which holds data for the evaluation and some configuration for CCom.

#### Source file
Here is an example YAML file, which can be evaluated by CCom:
```yaml
...
property1: "value1"
#! if has service angular | has service mysql {
#! property2:
#! 	property3: true
#! }
...
```

Another example for Java (whatever you want to achive with that)
```java
public class Example {
	public static void main(String[] args) {
		//! if has property_name {
        //! System.out.println("True");
        //! }
		//! if not has property_name {
        //! System.out.println("False");
        //! }
	}
}
```

#### Data input
```json
{
    "comment-char": "#",
    "delete-comments-on-false": true,
    "data": {
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
}
```

### Single condition + data input => boolean output
You also have the option to feed CCom with a single condition and get a boolean output, whether this condition is true or false.
```ccom
has service angular | has service mysql | has backend
```

In combination with the data input from above, this condition would be `false`, because the data collection does not fulfill any of the three sub-conditions.

## Keywords
-   `if`
-   `has`
-   `contains`

## Special characters
-   `!`
-   `|`
-   `{` / `}`

## Usage

*To be extended...*

## Contribute otherwise to the project
If you want to contribute to this project, please ensure you comply with the contribution guidelines.

© Marc Auberer 2021