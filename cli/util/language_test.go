package util

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

// ------------------------------------- GetCommentIdenFromLang -------------------------------------

func TestGetComIdenFromLang_Success1(t *testing.T) {
	// Manual input
	langList := []string{"yaml", "yml", "docker", "dockerfile"}
	for _, lang := range langList {
		lineComIden, blockComIdenOpen, blockComIdenClose := GetCommentIdenFromLang(lang, "")
		assert.Equal(t, "#", lineComIden)
		assert.Empty(t, blockComIdenOpen)
		assert.Empty(t, blockComIdenClose)
	}
	// Auto
	langList = []string{"yaml", "yml"}
	for _, lang := range langList {
		lineComIden, blockComIdenOpen, blockComIdenClose := GetCommentIdenFromLang("auto", "./test-files/test."+lang)
		assert.Equal(t, "#", lineComIden)
		assert.Empty(t, blockComIdenOpen)
		assert.Empty(t, blockComIdenClose)
	}
}

func TestGetComIdenFromLang_Success2(t *testing.T) {
	// Manual input
	langList := []string{"python", "py"}
	for _, lang := range langList {
		lineComIden, blockComIdenOpen, blockComIdenClose := GetCommentIdenFromLang(lang, "")
		assert.Equal(t, "#", lineComIden)
		assert.Equal(t, "\"\"\"", blockComIdenOpen)
		assert.Equal(t, "\"\"\"", blockComIdenClose)
	}
	// Auto
	langList = []string{"py"}
	for _, lang := range langList {
		lineComIden, blockComIdenOpen, blockComIdenClose := GetCommentIdenFromLang("auto", "./test-files/test."+lang)
		assert.Equal(t, "#", lineComIden)
		assert.Equal(t, "\"\"\"", blockComIdenOpen)
		assert.Equal(t, "\"\"\"", blockComIdenClose)
	}
}

func TestGetComIdenFromLang_Success3(t *testing.T) {
	// Manual
	langList := []string{"java", "c", "c++", "cpp", "golang", "go", "javascript", "js", "typescript", "ts", "rust", "rs"}
	for _, lang := range langList {
		lineComIden, blockComIdenOpen, blockComIdenClose := GetCommentIdenFromLang(lang, "")
		assert.Equal(t, "//", lineComIden)
		assert.Equal(t, "/*", blockComIdenOpen)
		assert.Equal(t, "*/", blockComIdenClose)
	}
	// Auto
	langList = []string{"java", "go", "js", "ts", "rs"}
	for _, lang := range langList {
		lineComIden, blockComIdenOpen, blockComIdenClose := GetCommentIdenFromLang("auto", "./test-files/test."+lang)
		assert.Equal(t, "//", lineComIden)
		assert.Equal(t, "/*", blockComIdenOpen)
		assert.Equal(t, "*/", blockComIdenClose)
	}
}

func TestGetComIdenFromLang_Success4(t *testing.T) {
	langList := []string{"html", "htm", "xml"}
	// Manual
	for _, lang := range langList {
		lineComIden, blockComIdenOpen, blockComIdenClose := GetCommentIdenFromLang(lang, "")
		assert.Empty(t, lineComIden)
		assert.Equal(t, "<!--", blockComIdenOpen)
		assert.Equal(t, "-->", blockComIdenClose)
	}
	// Auto
	for _, lang := range langList {
		lineComIden, blockComIdenOpen, blockComIdenClose := GetCommentIdenFromLang("auto", "./test-files/test."+lang)
		assert.Empty(t, lineComIden)
		assert.Equal(t, "<!--", blockComIdenOpen)
		assert.Equal(t, "-->", blockComIdenClose)
	}
}
