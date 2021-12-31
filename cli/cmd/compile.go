/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

package cmd

import (
	"ccom/util"
	"io/ioutil"

	"github.com/urfave/cli/v2"
)

// CompileCliFlags are the cli flags for the compile command
var CompileCliFlags = []cli.Flag{
	&cli.StringFlag{
		Name:    "block-comment-iden-open",
		Aliases: []string{"bcio"},
		Usage:   "Specifies the opening block comment identifier(s) of your data format",
	},
	&cli.StringFlag{
		Name:    "block-comment-iden-close",
		Aliases: []string{"bcic"},
		Usage:   "Specifies the closing block comment identifier(s) of your data format",
	},
	/*&cli.StringFlag{
		Name:        "compiler",
		Aliases:     []string{"c"},
		DefaultText: "cpp",
		Usage:       "CCom supports several compiler implementations, which you can choose from. Possible options are: 'cpp' (default), 'java', more to come ...",
	},*/
	&cli.StringFlag{
		Name:        "data",
		Aliases:     []string{"d"},
		DefaultText: "{}",
		Usage:       "JSON string or path to JSON file, which holds the evaluation work data",
	},
	&cli.BoolFlag{
		Name:    "force",
		Aliases: []string{"f"},
		Usage:   "Ignore safety checks. Warning: This could cause demage",
	},
	&cli.StringFlag{
		Name:        "lang",
		Aliases:     []string{"l"},
		DefaultText: "auto",
		Usage:       "File format / programming language (e.g.: yaml, java, html, ...)",
	},
	&cli.StringFlag{
		Name:    "line-comment-iden",
		Aliases: []string{"lci"},
		Usage:   "Specifies the line comment identifier(s) of your data format",
	},
	&cli.BoolFlag{
		Name:    "mode-single",
		Aliases: []string{"m"},
		Usage:   "Set input mode to single statement list",
	},
	&cli.StringFlag{
		Name:    "out-file",
		Aliases: []string{"o"},
		Usage:   "Path to output file. If you omit this flag, the output will be printed to the console",
	},
	&cli.BoolFlag{
		Name:    "silent",
		Aliases: []string{"s"},
		Usage:   "Only print raw compiler output and no debug output",
	},
}

// Compile compiles a source file or a single condition
func Compile(c *cli.Context) error {
	// Extract flags
	fileInput := c.Args().Get(0)
	compiler := /*c.String("compiler")*/ "cpp"
	comBlockIdenOpen := c.String("block-comment-iden-open")
	comBlockIdenClose := c.String("block-comment-iden-close")
	comLineIden := c.String("line-comment-iden")
	dataInput := c.String("data")
	forceFlag := c.Bool("force")
	lang := c.String("lang")
	modeSingle := c.Bool("mode-single")
	outFile := c.String("out-file")
	silentFlag := c.Bool("silent")

	// Analyze correctness of inputs
	if !silentFlag {
		util.P("Analyzing inputs ... ")
	}
	util.EnsureInput(&fileInput, &dataInput, &compiler, lang, &comLineIden, &comBlockIdenOpen, &comBlockIdenClose, modeSingle)
	if !silentFlag {
		util.Done()
	}

	// Feed the compiler with the input
	if !silentFlag {
		util.P("Compiling ... ")
	}

	result := util.RunCompilerExecutable(compiler, modeSingle, fileInput, dataInput, comLineIden, comBlockIdenOpen, comBlockIdenClose)

	if !silentFlag {
		util.Done()
	}

	// Write output
	if outFile != "" { // To file
		// Check if output file exists
		if util.FileExists(outFile) && !forceFlag {
			// Ask user if he wants to overwrite the output file
			if !util.YesNoQuestion("The output file already exists. Do you want to overwrite it?", false) {
				return nil
			}
		}
		// Write output to file
		if err := ioutil.WriteFile(outFile, []byte(result), 0600); err != nil {
			util.Error("Could not write output file", 1)
		}
	} else { // Print to console
		if !silentFlag {
			util.Pel()
		}
		util.Pl(result)
	}

	return nil
}
