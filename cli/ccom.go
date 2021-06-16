package main

import (
	"ccom/util"
	"log"
	"os"

	"github.com/urfave/cli/v2"
)

// nolint: gochecknoglobals
var (
	version = "dev"
	commit  = ""
	date    = ""
	builtBy = ""
)

func main() {
	// Version flag
	cli.VersionFlag = &cli.BoolFlag{
		Name:    "version",
		Aliases: []string{"v"},
		Usage:   "Prints the version of the ccom cli",
	}

	// Main cli configuration
	app := &cli.App{
		Name:    "ccom",
		Version: util.BuildVersion(version, commit, date, builtBy),
		Authors: []*cli.Author{
			{Name: "Marc Auberer", Email: "marc.auberer@chillibits.com"},
		},
		Copyright: "© 2021 Marc Auberer",
		Usage:     "Evaluate conditional comment sections of data files.",
		Flags: []cli.Flag{
			&cli.IntFlag{
				Name:    "benchmark",
				Aliases: []string{"b"},
				Usage:   "Benchmark the specified compiler executable and run it n times",
			},
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
			&cli.StringFlag{
				Name:        "compiler",
				Aliases:     []string{"c"},
				DefaultText: "cpp",
				Usage:       "CCom supports several compiler implementations, which you can choose from. Possible options are: 'cpp' (default), 'java', more to come ...",
			},
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
		},
		Action: func(c *cli.Context) error {
			processInput(
				c.Args().Get(0),
				c.String("compiler"),
				c.Int("benchmark"),
				c.String("block-comment-iden-open"),
				c.String("block-comment-iden-close"),
				c.String("data"),
				c.Bool("force"),
				c.String("lang"),
				c.String("line-comment-iden"),
				c.Bool("mode-single"),
				c.String("out-file"),
				c.Bool("silent"),
			)
			return nil
		},
		UseShortOptionHandling: true,
	}

	err := app.Run(os.Args)
	if err != nil {
		log.Fatal(err)
	}
}
