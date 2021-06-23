package main

import (
	"ccom/util"
	"io/ioutil"
	"path/filepath"
	"strconv"
	"strings"
	"time"

	"github.com/kardianos/osext"
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
		util.P("Analyzing inputs ... ")
	}
	analyze(&fileInput, &dataInput, &compiler, lang, &lineCommentChars, &blockCommentCharsOpen, &blockCommentCharsClose, modeSingle)
	if !silentFlag {
		util.Done()
	}

	if benchmarkRuns <= 0 { // Normal compiler mode
		// Feed the compiler with the input
		if !silentFlag {
			util.P("Compiling ... ")
		}

		result := runCompilerExecutable(compiler, modeSingle, fileInput, dataInput, lineCommentChars, blockCommentCharsOpen, blockCommentCharsClose)

		if !silentFlag {
			util.Done()
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
				util.Pel()
			}
			util.Pl(result)
		}
	} else { // Benchmarking mode
		overallRuntime := 0
		for i := 1; i <= benchmarkRuns; i++ {
			start := time.Now()
			runCompilerExecutable(compiler, modeSingle, fileInput, dataInput, lineCommentChars, blockCommentCharsOpen, blockCommentCharsClose)
			duration := time.Since(start).Milliseconds()
			overallRuntime += int(duration)
			util.Pl(strconv.Itoa(i) + ". run: " + strconv.Itoa(int(duration)) + " ms")
		}
		util.Pel()
		util.Info("Overall runtime: " + strconv.Itoa(overallRuntime) + " ms")
		util.Info("Average runtime: " + strconv.Itoa(overallRuntime/benchmarkRuns) + " ms")
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

	// Get raw data strings
	if !modeSingle {
		// Ensure value of comment char
		if *lineCommentIden == "" && *blockCommentIdenOpen == "" && *blockCommentIdenClose == "" {
			*lineCommentIden, *blockCommentIdenOpen, *blockCommentIdenClose = getCommentIdenFromLang(lang, *fileInput)
		} else if (*blockCommentIdenOpen == "" && *blockCommentIdenClose != "") || (*blockCommentIdenOpen != "" && *blockCommentIdenClose == "") {
			util.Error("You cannot specify only one of blockCommentIdenOpen and blockCommentIdenClose. Please specify both or none.", true)
		}

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
	// Determine executeable path based on the environment
	executablePath, _ := osext.Executable()
	executablePath = strings.ReplaceAll(executablePath, "\\", "/")
	executablePath = executablePath[:strings.LastIndex(executablePath, "/")] + "/"
	if util.FileExists("/usr/lib/ccom") {
		executablePath = "/usr/lib/ccom/"
	}

	// Determine executeable name by name of compiler
	executableName := "ccomc"
	switch compiler {
	case "cpp", "c++":
		executableName = "ccomc"
	case "java":
		executableName = "ccomc-java.jar"
	default:
		util.Error("Invalid compiler name. Only 'cpp' and 'java' are allowed values.", true)
	}
	// Check if executable exists
	if !util.CommandExists(executablePath + executableName) {
		util.Error("Compiler executable not found. Please check your installation", true)
	}
	// Execute compiler with user inputs
	return util.ExecuteAndWaitWithOutput(executablePath+executableName, strconv.FormatBool(modeSingle), fileInput, dataInput, lineCommentIden, blockCommentIdenOpen, blockCommentIdenClose)
}

func getCommentIdenFromLang(lang string, fileInput string) (lineCommentIden string, blockCommentIdenOpen string, blockCommentIdenClose string) {
	if lang == "" || lang == "auto" {
		if !util.FileExists(fileInput) {
			util.Error("Please use lang 'auto' only in combination of valid file paths as file input", true)
		}
		lang = filepath.Ext(fileInput)[1:]
	}

	switch lang {
	case "yaml", "yml", "docker", "dockerfile":
		lineCommentIden = "#"
		blockCommentIdenOpen = ""
		blockCommentIdenClose = ""
	case "python", "py", "elixir", "ex", "exs":
		lineCommentIden = "#"
		blockCommentIdenOpen = "\"\"\""
		blockCommentIdenClose = "\"\"\""
	case "java", "c", "c++", "cpp", "golang", "go", "javascript", "js", "typescript", "ts", "rust", "rs", "swift", "php", "kotlin", "kt", "dart", "groovy":
		lineCommentIden = "//"
		blockCommentIdenOpen = "/*"
		blockCommentIdenClose = "*/"
	case "html", "htm", "xml":
		lineCommentIden = ""
		blockCommentIdenOpen = "<!--"
		blockCommentIdenClose = "-->"
	case "haskell", "hs":
		lineCommentIden = "--"
		blockCommentIdenOpen = "{-"
		blockCommentIdenClose = "-}"
	case "lua":
		lineCommentIden = "--"
		blockCommentIdenOpen = "--[["
		blockCommentIdenClose = "]]"
	case "pascal", "pas":
		lineCommentIden = ""
		blockCommentIdenOpen = "(*"
		blockCommentIdenClose = "*)"
	case "perl", "pl":
		lineCommentIden = "#"
		blockCommentIdenOpen = "=item"
		blockCommentIdenClose = "=cut"
	case "r":
		lineCommentIden = "#"
		blockCommentIdenOpen = ""
		blockCommentIdenClose = ""
	case "powershell", "ps":
		lineCommentIden = "#"
		blockCommentIdenOpen = "<#"
		blockCommentIdenClose = "#>"
	case "ruby", "rb":
		lineCommentIden = "#"
		blockCommentIdenOpen = "=begin"
		blockCommentIdenClose = "=end"
	case "sql":
		lineCommentIden = "--"
		blockCommentIdenOpen = ""
		blockCommentIdenClose = ""
	case "julia", "jl":
		lineCommentIden = "#"
		blockCommentIdenOpen = "#="
		blockCommentIdenClose = "=#"
	default:
		util.Error("Unknown lang", true)
	}
	return
}

func ensureFileInputString(text *string) {
	// Is it a path to a file
	if util.FileExists(*text) && !util.IsDir(*text) {
		result, err := util.GetFileContents(*text)
		if err != nil {
			util.Error("Could not read input file", true)
		}
		*text = result
	}
}

func ensureDataInputString(text *string) {
	// Is it a path to a file
	if util.FileExists(*text) && !util.IsDir(*text) {
		result, err := util.GetFileContents(*text)
		if err != nil {
			util.Error("Could not read data file", true)
		}
		*text = result
	}
	// If it is json
	if util.IsJson(*text) {
		return
	}
	// Noting => throw error
	util.Error("The data file parameter must be a valid filepath or a JSON string", true)
}
