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
