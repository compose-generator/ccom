package util

import (
	"os"
	"os/exec"
	"strconv"
	"strings"
	"syscall"

	"github.com/AlecAivazis/survey/v2"
	"github.com/AlecAivazis/survey/v2/terminal"
	"github.com/cli/safeexec"
)

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
