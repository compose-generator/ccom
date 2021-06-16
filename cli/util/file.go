package util

import (
	"io/ioutil"
	"os"
)

// GetFileContents reads a file and returns its content as a string
func GetFileContents(path string) (string, error) {
	b, err := ioutil.ReadFile(path)
	if err != nil {
		return "", err
	}
	return string(b), nil
}

// FileExists checks if a file or directory exists
func FileExists(path string) bool {
	_, err := os.Stat(path)
	if err != nil {
		return false
	}
	return !os.IsNotExist(err)
}

// IsDir checks if a file is a directory
func IsDir(path string) bool {
	fileInfo, err := os.Stat(path)
	if err != nil {
		return false
	}
	return fileInfo.IsDir()
}
