package main

import (
	"ccom/util"
	"errors"
	"fmt"
	"io/ioutil"
	"log"
	"path/filepath"
	"strconv"
	"strings"
	"time"
)

func processInput(
	fileInput string,
	compiler string,
	benchmarkRuns int,
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
	analyze(&fileInput, &dataInput, &compiler, lang, &lineCommentChars, &blockCommentCharsOpen, &blockCommentCharsClose, modeSingle)
	if !silentFlag {
		fmt.Println("done")
	}

	if benchmarkRuns <= 0 { // Normal compiler mode
		// Feed the compiler with the input
		if !silentFlag {
			fmt.Print("Compiling ... ")
		}

		result := runCompilerExecutable(compiler, modeSingle, fileInput, dataInput, lineCommentChars, blockCommentCharsOpen, blockCommentCharsClose)

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
	} else { // Benchmarking mode
		overallRuntime := 0
		for i := 1; i <= benchmarkRuns; i++ {
			start := time.Now()
			runCompilerExecutable(compiler, modeSingle, fileInput, dataInput, lineCommentChars, blockCommentCharsOpen, blockCommentCharsClose)
			duration := time.Since(start).Milliseconds()
			overallRuntime += int(duration)
			fmt.Println(strconv.Itoa(i) + ". run: " + strconv.Itoa(int(duration)) + " ms")
		}
		fmt.Println()
		fmt.Println("Overall runtime: " + strconv.Itoa(overallRuntime) + " ms")
	}
}

func analyze(
	fileInput *string,
	dataInput *string,
	compiler *string,
	lang string,
	lineCommentIden *string,
	blockCommentIdenOpen *string,
	blockCommentIdenClose *string,
	modeSingle bool,
) {
	// Ensure value of input data
	if *dataInput == "" {
		*dataInput = "{}"
	}
	if *compiler == "" {
		*compiler = "cpp"
	}
	if lang != "" {
		*lineCommentIden, *blockCommentIdenOpen, *blockCommentIdenClose = getCommentIdenFromLang(lang, *fileInput)
	}
	// Ensure value of comment char
	if *lineCommentIden == "" && *blockCommentIdenOpen == "" && *blockCommentIdenClose == "" {
		*lineCommentIden = "#"
		*blockCommentIdenOpen = ""
		*blockCommentIdenClose = ""
	}
	if (*blockCommentIdenOpen == "" && *blockCommentIdenClose != "") || (*blockCommentIdenOpen != "" && *blockCommentIdenClose == "") {
		log.Fatal("You cannot specify only one of blockCommentIdenOpen and blockCommentIdenClose. Please specify both or none.")
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

func runCompilerExecutable(
	compiler string,
	modeSingle bool,
	fileInput string,
	dataInput string,
	lineCommentIden string,
	blockCommentIdenOpen string,
	blockCommentIdenClose string,
) string {
	// Determine executeable name by name of compiler
	executableName := "./ccomc"
	switch compiler {
	case "cpp", "c++":
		executableName = "./ccomc"
	case "java":
		executableName = "./ccomc-java.jar"
	default:
		log.Fatal("Invalid compiler name. Only 'cpp' and 'java' are allowed values.")
	}
	// Check if executable exists
	if !util.CommandExists(executableName) {
		log.Fatal("Compiler executable not found. Please check your installation")
	}
	// Execute compiler with user inputs
	return util.ExecuteAndWaitWithOutput(executableName, strconv.FormatBool(modeSingle), fileInput, dataInput, lineCommentIden, blockCommentIdenOpen, blockCommentIdenClose)
}

func getCommentIdenFromLang(lang string, fileInput string) (lineCommentIden string, blockCommentIdenOpen string, blockCommentIdenClose string) {
	if lang == "auto" {
		if !util.FileExists(fileInput) {
			log.Fatal("Please use lang 'auto' only in combination of valid file paths as file input")
		}
		lang = filepath.Ext(fileInput)[1:]
	}

	switch lang {
	case "yaml", "yml", "python", "py", "docker", "dockerfile":
		lineCommentIden = "#"
		blockCommentIdenOpen = ""
		blockCommentIdenClose = ""
	case "java", "c", "c++", "cpp", "golang", "go", "javascript", "js", "typescript", "ts", "rust", "rs":
		lineCommentIden = "//"
		blockCommentIdenOpen = "/*"
		blockCommentIdenClose = "*/"
	case "html", "htm", "xml":
		lineCommentIden = ""
		blockCommentIdenOpen = "<!--"
		blockCommentIdenClose = "-->"
	default:
		log.Fatal("Unknown lang")
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
