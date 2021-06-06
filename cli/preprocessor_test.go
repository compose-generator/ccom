package main

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

// ------------------------------------- getCommentIdenFromLang -------------------------------------

func TestGetComIdenFromLang_Success1(t *testing.T) {
	// Manual input
	langList := []string{"yaml", "yml", "docker", "dockerfile"}
	for _, lang := range langList {
		lineComIden, blockComIdenOpen, blockComIdenClose := getCommentIdenFromLang(lang, "")
		assert.Equal(t, "#", lineComIden)
		assert.Empty(t, blockComIdenOpen)
		assert.Empty(t, blockComIdenClose)
	}
	// Auto
	langList = []string{"yaml", "yml"}
	for _, lang := range langList {
		lineComIden, blockComIdenOpen, blockComIdenClose := getCommentIdenFromLang("auto", "./test-files/test."+lang)
		assert.Equal(t, "#", lineComIden)
		assert.Empty(t, blockComIdenOpen)
		assert.Empty(t, blockComIdenClose)
	}
}

func TestGetComIdenFromLang_Success2(t *testing.T) {
	// Manual input
	langList := []string{"python", "py"}
	for _, lang := range langList {
		lineComIden, blockComIdenOpen, blockComIdenClose := getCommentIdenFromLang(lang, "")
		assert.Equal(t, "#", lineComIden)
		assert.Equal(t, "\"\"\"", blockComIdenOpen)
		assert.Equal(t, "\"\"\"", blockComIdenClose)
	}
	// Auto
	langList = []string{"py"}
	for _, lang := range langList {
		lineComIden, blockComIdenOpen, blockComIdenClose := getCommentIdenFromLang("auto", "./test-files/test."+lang)
		assert.Equal(t, "#", lineComIden)
		assert.Equal(t, "\"\"\"", blockComIdenOpen)
		assert.Equal(t, "\"\"\"", blockComIdenClose)
	}
}

func TestGetComIdenFromLang_Success3(t *testing.T) {
	// Manual
	langList := []string{"java", "c", "c++", "cpp", "golang", "go", "javascript", "js", "typescript", "ts", "rust", "rs"}
	for _, lang := range langList {
		lineComIden, blockComIdenOpen, blockComIdenClose := getCommentIdenFromLang(lang, "")
		assert.Equal(t, "//", lineComIden)
		assert.Equal(t, "/*", blockComIdenOpen)
		assert.Equal(t, "*/", blockComIdenClose)
	}
	// Auto
	langList = []string{"java", "c", "cpp", "go", "js", "ts", "rs"}
	for _, lang := range langList {
		lineComIden, blockComIdenOpen, blockComIdenClose := getCommentIdenFromLang("auto", "./test-files/test."+lang)
		assert.Equal(t, "//", lineComIden)
		assert.Equal(t, "/*", blockComIdenOpen)
		assert.Equal(t, "*/", blockComIdenClose)
	}
}

func TestGetComIdenFromLang_Success4(t *testing.T) {
	langList := []string{"html", "htm", "xml"}
	// Manual
	for _, lang := range langList {
		lineComIden, blockComIdenOpen, blockComIdenClose := getCommentIdenFromLang(lang, "")
		assert.Empty(t, lineComIden)
		assert.Equal(t, "<!--", blockComIdenOpen)
		assert.Equal(t, "-->", blockComIdenClose)
	}
	// Auto
	for _, lang := range langList {
		lineComIden, blockComIdenOpen, blockComIdenClose := getCommentIdenFromLang("auto", "./test-files/test."+lang)
		assert.Empty(t, lineComIden)
		assert.Equal(t, "<!--", blockComIdenOpen)
		assert.Equal(t, "-->", blockComIdenClose)
	}
}

// -------------------------------------- ensureFileInputString -------------------------------------

func TestEnsureFileInputString_File(t *testing.T) {
	testInput := "./test-files/test.txt"
	ensureFileInputString(&testInput)
	assert.Equal(t, "This is a test content", testInput)
}

func TestEnsureFileInputString_String(t *testing.T) {
	testInput := "Test input"
	ensureFileInputString(&testInput)
	assert.Equal(t, "Test input", testInput)
}

// -------------------------------------- ensureDataInputString -------------------------------------

func TestEnsureDataInputString_File(t *testing.T) {
	testData := "./test-files/test-data.json"
	ensureDataInputString(&testData)
	assert.Equal(t, "{\"attribute1\": true, \"attribute2\": \"test\"}", testData)
}

func TestEnsureDataInputString_String(t *testing.T) {
	testData := "{ \"attribute1\": true,\n\"attribute2\": \"test\" }"
	ensureDataInputString(&testData)
	assert.Equal(t, "{ \"attribute1\": true,\n\"attribute2\": \"test\" }", testData)
}
