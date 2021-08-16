package util

import (
	"fmt"
	"os"

	"github.com/fatih/color"
)

// P prints text in white to the console without a line break at the end
func P(text string) {
	_, err := color.New(color.FgWhite).Print(text)
	if err != nil {
		fmt.Println(text)
	}
}

// Pl prints text in white to the console with a like break at the end
func Pl(text string) {
	color.White(text)
}

// Pel prints an empty line to the console
func Pel() {
	fmt.Println()
}

// Info prints text in yellow to the console
func Info(text string) {
	color.Yellow(text)
}

// Done prints 'done' in green to the console
func Done() {
	color.Green("done")
}

// Error prints an error message in red to the console and exits if the exit flag was set
func Error(message string, exitCode int) {
	color.Red(message)
	if exitCode != 0 {
		os.Exit(exitCode)
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
