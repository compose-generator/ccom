package util

import (
	"encoding/json"
	"fmt"
)

// IsJson checks if the input string is a valid json structure
func IsJson(s string) bool {
	var js map[string]interface{}
	return json.Unmarshal([]byte(s), &js) == nil
}

// BuildVersion takes individual version component strings and merges it to a version string output
func BuildVersion(version, commit, date, builtBy string) string {
	result := version
	if commit != "" {
		result = fmt.Sprintf("%s, commit: %s", result, commit)
	}
	if date != "" {
		result = fmt.Sprintf("%s, built at: %s", result, date)
	}
	if builtBy != "" {
		result = fmt.Sprintf("%s, built by: %s", result, builtBy)
	}
	return result
}
