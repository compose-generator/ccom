package main

import (
	"ccom/util"
	"errors"
	"fmt"
	"log"
	"strings"
)

func processInput(
	fileInput string,
	dataInput string,
	lang string,
	lineCommentChars string,
	blockCommentCharsOpen string,
	blockCommentCharsClose string,
	preserveFlag bool,
) {
	// Analyze correctness of inputs
	fmt.Print("Analyzing inputs ... ")
	analyze(&fileInput, &dataInput, lang, &lineCommentChars, &blockCommentCharsOpen, &blockCommentCharsClose)
	fmt.Println("done")

	// Feed the compiler with the individual sections
	fmt.Print("Compiling ... ")
	result := util.ExecuteAndWaitWithOutput("./ccomc", fileInput, dataInput, lineCommentChars, blockCommentCharsOpen, blockCommentCharsClose)
	fmt.Println("done")

	// Temporarily print preprocessed output
	fmt.Println(result)
}

func analyze(fileInput *string, dataInput *string, lang string, lineCommentChars *string, blockCommentCharsOpen *string, blockCommentCharsClose *string) {
	// Ensure value of input data
	if *dataInput == "" {
		*dataInput = "{}"
	}
	if lang != "" {
		*lineCommentChars, *blockCommentCharsOpen, *blockCommentCharsClose = getCommentCharsFromLang(lang)
	}
	// Ensure value of comment char
	if *lineCommentChars == "" && *blockCommentCharsOpen == "" && *blockCommentCharsClose == "" {
		*lineCommentChars = "#"
		*blockCommentCharsOpen = ""
		*blockCommentCharsClose = ""
	}
	// Get raw data strings
	ensureInputString(fileInput)
	ensureDataString(dataInput)

	// Replace any Windows line breaks with Linux line breaks
	*fileInput = strings.ReplaceAll(*fileInput, "\r\n", "\n")
	*dataInput = strings.ReplaceAll(*dataInput, "\r\n", "\n")
}

func getCommentCharsFromLang(lang string) (lineCommentChars string, blockCommentCharsOpen string, blockCommentCharsClose string) {
	switch lang {
	case "yaml", "yml", "python", "docker", "dockerfile":
		lineCommentChars = "#"
		blockCommentCharsOpen = ""
		blockCommentCharsClose = ""
	case "java", "c", "c++", "cpp", "golang", "go", "javascript", "js", "typescript", "ts", "rust":
		lineCommentChars = "//"
		blockCommentCharsOpen = "/*"
		blockCommentCharsClose = "*/"
	case "html", "xml":
		lineCommentChars = ""
		blockCommentCharsOpen = "<!--"
		blockCommentCharsClose = "-->"
	}
	return
}

func ensureInputString(text *string) {
	// Is it a path to a file
	if util.FileExists(*text) && !util.IsDir(*text) {
		*text = util.GetFileContents(*text)
	}
}

func ensureDataString(text *string) {
	// Is it a path to a file
	if util.FileExists(*text) && !util.IsDir(*text) {
		*text = util.GetFileContents(*text)
	}
	// If it is json
	if util.IsJson(*text) {
		return
	}
	// Noting => throw error
	log.Fatal(errors.New("The data file parameter must be a valid filepath or a JSON string"))
}
