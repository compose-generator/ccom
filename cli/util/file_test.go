/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

package util

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

// ------------------------------------------ GetFileContents ------------------------------------------

func TestGetFileContents_Success(t *testing.T) {
	result, err := GetFileContents("../test-files/test.txt")
	assert.Nil(t, err)
	assert.Equal(t, "This is a test content", result)
}

func TestGetFileContents_Failure1(t *testing.T) {
	result, err := GetFileContents("../test-files/test.txt")
	assert.Nil(t, err)
	assert.NotEqual(t, "Wrong content", result)
}

func TestGetFileContents_Failure2(t *testing.T) {
	result, err := GetFileContents("../test-files/non-existing.txt")
	assert.NotNil(t, err)
	assert.Empty(t, result)
}

// -------------------------------------------- FileExists ---------------------------------------------

func TestFileExists_Success(t *testing.T) {
	result := FileExists("../test-files/test.txt")
	assert.True(t, result)
}

func TestFileExists_Failure(t *testing.T) {
	result := FileExists("../test-files/non-existing.txt")
	assert.False(t, result)
}

// ----------------------------------------------- IsDir -----------------------------------------------

func TestIsDir_Failure(t *testing.T) {
	result := FileExists("../non-existing")
	assert.False(t, result)
}

func TestIsDir_Success(t *testing.T) {
	result := FileExists("../test-files")
	assert.True(t, result)
}
