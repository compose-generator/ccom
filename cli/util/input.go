package util

import "strings"

func EnsureInput(
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
			*lineCommentIden, *blockCommentIdenOpen, *blockCommentIdenClose = GetCommentIdenFromLang(lang, *fileInput)
		} else if (*blockCommentIdenOpen == "" && *blockCommentIdenClose != "") || (*blockCommentIdenOpen != "" && *blockCommentIdenClose == "") {
			Error("You cannot specify only one of blockCommentIdenOpen and blockCommentIdenClose. Please specify both or none.", 1)
		}

		ensureFileInputString(fileInput)
	}
	ensureDataInputString(dataInput)

	// Replace any Windows line breaks with Linux line breaks
	*fileInput = strings.ReplaceAll(*fileInput, "\r\n", "\n")
	*dataInput = strings.ReplaceAll(*dataInput, "\r\n", "\n")
}

func ensureFileInputString(text *string) {
	// Is it a path to a file
	if FileExists(*text) && !IsDir(*text) {
		result, err := GetFileContents(*text)
		if err != nil {
			Error("Could not read input file", 1)
		}
		*text = result
	}
}

func ensureDataInputString(text *string) {
	// Is it a path to a file
	if FileExists(*text) && !IsDir(*text) {
		result, err := GetFileContents(*text)
		if err != nil {
			Error("Could not read data file", 1)
		}
		*text = result
	}
	// If it is json
	if IsJson(*text) {
		return
	}
	// Noting => throw error
	Error("The data file parameter must be a valid filepath or a JSON string", 1)
}
