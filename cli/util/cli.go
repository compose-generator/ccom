/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

package util

import (
	"os"
	"os/exec"
	"strconv"
	"strings"
	"syscall"

	"github.com/kardianos/osext"

	"github.com/AlecAivazis/survey/v2"
	"github.com/AlecAivazis/survey/v2/terminal"
	"github.com/cli/safeexec"
)

// RunCompilerExecutable passes all inputs to the compiler executable and returns with the result string
func RunCompilerExecutable(
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
	if FileExists("/usr/lib/ccom") {
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
		Error("Invalid compiler name. Only 'cpp' and 'java' are allowed values.", 1)
	}
	// Check if executable exists
	if !CommandExists(executablePath + executableName) {
		Error("Compiler executable not found. Please check your installation", 1)
	}
	// Execute compiler with user inputs
	return ExecuteAndWaitWithOutput(executablePath+executableName, strconv.FormatBool(modeSingle), fileInput, dataInput, lineCommentIden, blockCommentIdenOpen, blockCommentIdenClose)
}

// ExecuteAndWaitWithOutput executes a command and return the command output as string
func ExecuteAndWaitWithOutput(c ...string) string {
	cmd := exec.Command(c[0], c[1:]...)
	output, err := cmd.CombinedOutput()
	if err != nil {
		if exiterr, ok := err.(*exec.ExitError); ok {
			if status, ok := exiterr.Sys().(syscall.WaitStatus); ok {
				Pel()
				Pel()
				Error("Compiler exited with status code "+strconv.Itoa(status.ExitStatus())+"\nFailed to compile: "+string(output), status.ExitStatus())
			}
		} else {
			Error("Failed to call compiler executable", 1)
		}
	}
	return strings.TrimRight(string(output), "\r\n")
}

// CommandExists checks if the stated command exists on the host system
func CommandExists(cmd string) bool {
	_, err := safeexec.LookPath(cmd)
	return err == nil
}

// YesNoQuestion prints simple yes/no question with default value
func YesNoQuestion(question string, defaultValue bool) (result bool) {
	prompt := &survey.Confirm{
		Message: question,
		Default: defaultValue,
	}
	handleInterrupt(survey.AskOne(prompt, &result))
	return
}

func handleInterrupt(err error) {
	if err == terminal.InterruptErr {
		Pel()
		os.Exit(0)
	} else if err != nil {
		panic(err)
	}
}
