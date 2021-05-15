package util

import (
	"io/ioutil"
	"log"
	"os"
)

// GetFileContents reads a file and returns its content as a string
func GetFileContents(path string) string {
	b, err := ioutil.ReadFile(path)
	if err != nil {
		log.Fatal("Could not read file " + path)
	}
	return string(b)
}

// FileExists checks if a file exists
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
