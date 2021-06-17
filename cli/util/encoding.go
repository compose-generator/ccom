package util

import "encoding/json"

// IsJson checks if the input string is a valid json structure
func IsJson(s string) bool {
	var js map[string]interface{}
	return json.Unmarshal([]byte(s), &js) == nil
}
