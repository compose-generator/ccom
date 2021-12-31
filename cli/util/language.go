/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

package util

import (
	"ccom/model"
	"path/filepath"
	"strings"
)

// GetCommentIdenFromLang returns values for lineComIden, blockComIdenOpen and blockComIdenClose
func GetCommentIdenFromLang(lang string, fileInput string) (lineCommentIden string, blockCommentIdenOpen string, blockCommentIdenClose string) {
	if lang == "" || lang == "auto" {
		if !FileExists(fileInput) {
			Error("Please use lang 'auto' only in combination of valid file paths as file input", 1)
		}
		if filepath.Ext(fileInput) != "" {
			lang = filepath.Ext(fileInput)[1:]
		} else {
			lang = filepath.Base(fileInput)
		}
	}

	language, found := getLanguageList()[strings.ToLower(lang)]
	if !found {
		Error("Unknown lang", 1)
	}
	lineCommentIden = language.LineComIden
	blockCommentIdenOpen = language.BlockComIdenOpen
	blockCommentIdenClose = language.BlockComIdenClose
	return
}

func getLanguageList() map[string]model.LanguageDescriptor {
	languages := make(map[string]model.LanguageDescriptor)

	// Assembly
	assembly := model.LanguageDescriptor{
		Name:              "Assembly",
		LineComIden:       ";",
		BlockComIdenOpen:  "",
		BlockComIdenClose: "",
	}
	languages["assembly"] = assembly
	languages["asm"] = assembly

	// C
	c := model.LanguageDescriptor{
		Name:              "C",
		LineComIden:       "//",
		BlockComIdenOpen:  "/*",
		BlockComIdenClose: "*/",
	}
	languages["c"] = c

	// C++
	cpp := model.LanguageDescriptor{
		Name:              "C++",
		LineComIden:       "//",
		BlockComIdenOpen:  "/*",
		BlockComIdenClose: "*/",
	}
	languages["c++"] = cpp
	languages["cpp"] = cpp

	// Dart
	dart := model.LanguageDescriptor{
		Name:              "Dart",
		LineComIden:       "//",
		BlockComIdenOpen:  "/*",
		BlockComIdenClose: "*/",
	}
	languages["dart"] = dart

	// Dockerfile
	dockerfile := model.LanguageDescriptor{
		Name:              "Dockerfile",
		LineComIden:       "#",
		BlockComIdenOpen:  "",
		BlockComIdenClose: "",
	}
	languages["docker"] = dockerfile
	languages["dockerfile"] = dockerfile

	// Elixir
	elixir := model.LanguageDescriptor{
		Name:              "Elixir",
		LineComIden:       "#",
		BlockComIdenOpen:  "\"\"\"",
		BlockComIdenClose: "\"\"\"",
	}
	languages["elixir"] = elixir
	languages["exs"] = elixir
	languages["ex"] = elixir

	// Go
	golang := model.LanguageDescriptor{
		Name:              "Go",
		LineComIden:       "//",
		BlockComIdenOpen:  "/*",
		BlockComIdenClose: "*/",
	}
	languages["golang"] = golang
	languages["go"] = golang

	// Groovy
	groovy := model.LanguageDescriptor{
		Name:              "Groovy",
		LineComIden:       "//",
		BlockComIdenOpen:  "/*",
		BlockComIdenClose: "*/",
	}
	languages["groovy"] = groovy

	// Haskell
	haskell := model.LanguageDescriptor{
		Name:              "Haskell",
		LineComIden:       "--",
		BlockComIdenOpen:  "-{",
		BlockComIdenClose: "-}",
	}
	languages["haskell"] = haskell
	languages["hs"] = haskell

	// HTML
	html := model.LanguageDescriptor{
		Name:              "HTML",
		LineComIden:       "",
		BlockComIdenOpen:  "<!--",
		BlockComIdenClose: "-->",
	}
	languages["html"] = html
	languages["htm"] = html

	// Java
	java := model.LanguageDescriptor{
		Name:              "Java",
		LineComIden:       "//",
		BlockComIdenOpen:  "/*",
		BlockComIdenClose: "*/",
	}
	languages["java"] = java

	// JavaScript
	javascript := model.LanguageDescriptor{
		Name:              "JavaScript",
		LineComIden:       "//",
		BlockComIdenOpen:  "/*",
		BlockComIdenClose: "*/",
	}
	languages["javascript"] = javascript
	languages["js"] = javascript

	// Julia
	julia := model.LanguageDescriptor{
		Name:              "Julia",
		LineComIden:       "#",
		BlockComIdenOpen:  "#=",
		BlockComIdenClose: "=#",
	}
	languages["julia"] = julia
	languages["jl"] = julia

	// Kotlin
	kotlin := model.LanguageDescriptor{
		Name:              "Kotlin",
		LineComIden:       "//",
		BlockComIdenOpen:  "/*",
		BlockComIdenClose: "*/",
	}
	languages["kotlin"] = kotlin
	languages["kt"] = kotlin

	// Lua
	lua := model.LanguageDescriptor{
		Name:              "Lua",
		LineComIden:       "--",
		BlockComIdenOpen:  "--[[",
		BlockComIdenClose: "]]",
	}
	languages["lua"] = lua

	// Pascal
	pascal := model.LanguageDescriptor{
		Name:              "Pascal",
		LineComIden:       "",
		BlockComIdenOpen:  "(*",
		BlockComIdenClose: "*)",
	}
	languages["pascal"] = pascal
	languages["pas"] = pascal

	// Perl
	perl := model.LanguageDescriptor{
		Name:              "Perl",
		LineComIden:       "#",
		BlockComIdenOpen:  "=item",
		BlockComIdenClose: "=cut",
	}
	languages["perl"] = perl
	languages["pl"] = perl

	// PHP
	php := model.LanguageDescriptor{
		Name:              "PHP",
		LineComIden:       "//",
		BlockComIdenOpen:  "/*",
		BlockComIdenClose: "*/",
	}
	languages["php"] = php

	// Powershell
	powershell := model.LanguageDescriptor{
		Name:              "Powershell",
		LineComIden:       "#",
		BlockComIdenOpen:  "<#",
		BlockComIdenClose: "#>",
	}
	languages["powershell"] = powershell
	languages["ps1"] = powershell

	// Python
	python := model.LanguageDescriptor{
		Name:              "Python",
		LineComIden:       "#",
		BlockComIdenOpen:  "\"\"\"",
		BlockComIdenClose: "\"\"\"",
	}
	languages["python"] = python
	languages["py"] = python

	// R
	r := model.LanguageDescriptor{
		Name:              "R",
		LineComIden:       "#",
		BlockComIdenOpen:  "",
		BlockComIdenClose: "",
	}
	languages["r"] = r

	// Ruby
	ruby := model.LanguageDescriptor{
		Name:              "Ruby",
		LineComIden:       "#",
		BlockComIdenOpen:  "=begin",
		BlockComIdenClose: "=end",
	}
	languages["ruby"] = ruby
	languages["rb"] = ruby

	// Rust
	rust := model.LanguageDescriptor{
		Name:              "Rust",
		LineComIden:       "//",
		BlockComIdenOpen:  "/*",
		BlockComIdenClose: "*/",
	}
	languages["rust"] = rust
	languages["rs"] = rust

	// Spice
	spice := model.LanguageDescriptor{
		Name:              "Spice",
		LineComIden:       "//",
		BlockComIdenOpen:  "/*",
		BlockComIdenClose: "*/",
	}
	languages["spice"] = spice

	// SQL
	sql := model.LanguageDescriptor{
		Name:              "SQL",
		LineComIden:       "--",
		BlockComIdenOpen:  "",
		BlockComIdenClose: "",
	}
	languages["sql"] = sql

	// Swift
	swift := model.LanguageDescriptor{
		Name:              "Swift",
		LineComIden:       "//",
		BlockComIdenOpen:  "/*",
		BlockComIdenClose: "*/",
	}
	languages["swift"] = swift

	// Typescript
	typescript := model.LanguageDescriptor{
		Name:              "TypeScript",
		LineComIden:       "//",
		BlockComIdenOpen:  "/*",
		BlockComIdenClose: "*/",
	}
	languages["typescript"] = typescript
	languages["ts"] = typescript

	// XML
	xml := model.LanguageDescriptor{
		Name:              "XML",
		LineComIden:       "",
		BlockComIdenOpen:  "<!--",
		BlockComIdenClose: "-->",
	}
	languages["xml"] = xml

	// YAML
	yaml := model.LanguageDescriptor{
		Name:              "YAML",
		LineComIden:       "#",
		BlockComIdenOpen:  "",
		BlockComIdenClose: "",
	}
	languages["yaml"] = yaml
	languages["yml"] = yaml

	return languages
}
