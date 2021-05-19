package main

import (
	"ccom/util"
	"errors"
	"fmt"
	"io/ioutil"
	"log"
	"strconv"
	"strings"
)

// -------------------------------------------------- Public functions --------------------------------------------------

func ProcessInput(
	fileInput string,
	blockCommentCharsOpen string,
	blockCommentCharsClose string,
	dataInput string,
	force bool,
	lang string,
	lineCommentChars string,
	modeSingle bool,
	outFile string,
	silentFlag bool,
) {
	// Analyze correctness of inputs
	if !silentFlag {
		fmt.Print("Analyzing inputs ... ")
	}
	analyze(&fileInput, &dataInput, lang, &lineCommentChars, &blockCommentCharsOpen, &blockCommentCharsClose, modeSingle)
	if !silentFlag {
		fmt.Println("done")
	}

	// Feed the compiler with the input
	if !silentFlag {
		fmt.Print("Compiling ... ")
	}
	result := util.ExecuteAndWaitWithOutput("./ccomc", strconv.FormatBool(modeSingle), fileInput, dataInput, lineCommentChars, blockCommentCharsOpen, blockCommentCharsClose)
	if !silentFlag {
		fmt.Println("done")
	}

	// Write output
	if outFile != "" { // To file
		// Check if output file exists
		if util.FileExists(outFile) && !force {
			// Ask user if he wants to overwrite the output file
			if !util.YesNoQuestion("The output file already exists. Do you want to overwrite it?", false) {
				return
			}
		}
		// Write output to file
		ioutil.WriteFile(outFile, []byte(result), 0777)
	} else { // Print to console
		if !silentFlag {
			fmt.Println()
		}
		fmt.Println(result)
	}
}

// ------------------------------------------------- Private functions --------------------------------------------------

func analyze(
	fileInput *string,
	dataInput *string,
	lang string,
	lineCommentChars *string,
	blockCommentCharsOpen *string,
	blockCommentCharsClose *string,
	modeSingle bool,
) {
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
	if !modeSingle {
		ensureFileInputString(fileInput)
	}
	ensureDataInputString(dataInput)

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

func ensureFileInputString(text *string) {
	// Is it a path to a file
	if util.FileExists(*text) && !util.IsDir(*text) {
		*text = util.GetFileContents(*text)
	}
}

func ensureDataInputString(text *string) {
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
