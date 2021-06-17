package util

import (
	"fmt"
	"os"

	"github.com/fatih/color"
)

func P(text string) {
	color.New(color.FgWhite).Print(text)
}

func Pl(text string) {
	color.White(text)
}

func Pel() {
	fmt.Println()
}

func Info(text string) {
	color.Yellow(text)
}

func Done() {
	color.Green("done")
}

func Error(message string, exit bool) {
	color.Red(message)
	if exit {
		os.Exit(1)
	}
}

// BuildVersion takes individual version component strings and merges it to a version string output
func BuildVersion(version, commit, date, builtBy string) string {
	result := version
	if commit != "" {
		result = fmt.Sprintf("%s, commit: %s", result, commit)
	}
	if date != "" {
		result = fmt.Sprintf("%s, built at: %s", result, date)
	}
	if builtBy != "" {
		result = fmt.Sprintf("%s, built by: %s", result, builtBy)
	}
	return result
}
