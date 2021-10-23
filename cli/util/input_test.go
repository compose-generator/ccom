package util

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

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
