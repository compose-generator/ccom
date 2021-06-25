package model

// LanguageDescriptor contains all comment identifiers for a specific language
type LanguageDescriptor struct {
	Name              string
	LineComIden       string
	BlockComIdenOpen  string
	BlockComIdenClose string
}
