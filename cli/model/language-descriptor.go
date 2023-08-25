/*
Copyright © 2021-2023 Compose Generator Contributors
All rights reserved.
*/

package model

// LanguageDescriptor contains all comment identifiers for a specific language
type LanguageDescriptor struct {
	Name              string
	LineComIden       string
	BlockComIdenOpen  string
	BlockComIdenClose string
}
