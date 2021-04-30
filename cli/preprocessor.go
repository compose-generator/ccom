package main

import (
	"ccom/util"
	"errors"
	"fmt"
	"log"
	"strings"
)

func processInput(inputFile string, inputData string, commentChar string, preserveFlag bool) {
	// Analyze correctness of inputs
	sections := analyze(&inputFile, &inputData, &commentChar)

	// Feed the compiler with the individual sections
	processSections(inputFile, inputData, sections, preserveFlag)
}

func analyze(inputFile *string, inputData *string, commentChar *string) []string {
	// Ensure value of input data
	if *inputData == "" {
		*inputData = "{}"
	}
	// Ensure value of comment char
	if *commentChar == "" {
		*commentChar = "#"
	}
	// Get raw data strings
	ensureInputString(inputFile)
	ensureDataString(inputData)

	// Remove any Windows line breaks
	*inputFile = strings.ReplaceAll(*inputFile, "\r\n", "\n")
	*inputData = strings.ReplaceAll(*inputData, "\r\n", "\n")

	// Get conditional sections from inputFile
	sections := []string{}
	currentSection := ""
	for _, line := range strings.Split(*inputFile, "\n") {
		if strings.HasPrefix(line, *commentChar+"! ") {
			// Found conditional line
			currentSection += line + "\n"
			if line == *commentChar+"! }" {
				sections = append(sections, strings.TrimSuffix(currentSection, "\n"))
				currentSection = ""
			}
		}
	}
	return sections
}

func processSections(inputFile string, inputData string, conditionalSections []string, preserveFlag bool) {
	// Call coompiler for each conditional section
	for _, section := range conditionalSections {
		result := util.ExecuteAndWaitWithOutput("./ccom", section, inputData)
		strings.ReplaceAll(inputFile, section, result)
	}

	// Temporarily print preprocessed output
	fmt.Println(inputFile)
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
