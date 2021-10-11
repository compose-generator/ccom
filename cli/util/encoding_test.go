/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

package util

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

// ----------------------------------------------- IsJson ----------------------------------------------

func TestIsJson_InvalidJson1(t *testing.T) {
	result := IsJson("{")
	assert.False(t, result)
}

func TestIsJson_InvalidJson2(t *testing.T) {
	result := IsJson("This is a test")
	assert.False(t, result)
}

func TestIsJson_InvalidJson3(t *testing.T) {
	result := IsJson("")
	assert.False(t, result)
}

func TestIsJson_InvalidJson4(t *testing.T) {
	result := IsJson("./this/is/a/test/path.txt")
	assert.False(t, result)
}

func TestIsJson_ValidJson(t *testing.T) {
	result := IsJson("{ \"attribut1\": true,\n\"attr2\": \"123\" }")
	assert.True(t, result)
}
