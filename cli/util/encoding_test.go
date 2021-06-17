package util

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
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

// -------------------------------------------- BuildVersion -------------------------------------------

func TestBuildVersion(t *testing.T) {
	for name, tt := range map[string]struct {
		version, commit, date, builtBy string
		out                            string
	}{
		"all empty": {
			out: "",
		},
		"complete": {
			version: "1.2.3",
			date:    "12/12/12",
			commit:  "aaaa",
			builtBy: "me",
			out:     "1.2.3, commit: aaaa, built at: 12/12/12, built by: me",
		},
		"only version": {
			version: "1.2.3",
			out:     "1.2.3",
		},
		"version and date": {
			version: "1.2.3",
			date:    "12/12/12",
			out:     "1.2.3, built at: 12/12/12",
		},
		"version, date, built by": {
			version: "1.2.3",
			date:    "12/12/12",
			builtBy: "me",
			out:     "1.2.3, built at: 12/12/12, built by: me",
		},
	} {
		tt := tt
		t.Run(name, func(t *testing.T) {
			require.Equal(t, tt.out, BuildVersion(tt.version, tt.commit, tt.date, tt.builtBy))
		})
	}
}
