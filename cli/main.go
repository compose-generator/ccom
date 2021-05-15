package main

import (
	"log"
	"os"

	"github.com/urfave/cli/v2"
)

func main() {
	const VERSION = "0.1.0"

	// Version flag
	cli.VersionFlag = &cli.BoolFlag{
		Name:    "version",
		Aliases: []string{"v"},
		Usage:   "Prints the version of the ccom cli",
	}

	// Main cli configuration
	app := &cli.App{
		Name:    "ccom",
		Version: VERSION,
		Authors: []*cli.Author{
			{Name: "Marc Auberer", Email: "marc.auberer@chillibits.com"},
		},
		Copyright: "© 2021 Marc Auberer",
		Usage:     "Evaluate conditional comment sections of data files.",
		Flags: []cli.Flag{
			&cli.StringFlag{
				Name:    "block-comment-chars-open",
				Aliases: []string{"bcco"},
				Usage:   "Specifies the opening block comment character(s) of your data format",
			},
			&cli.StringFlag{
				Name:    "block-comment-chars-close",
				Aliases: []string{"bccc"},
				Usage:   "Specifies the closing block comment character(s) of your data format",
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
				DefaultText: "yaml",
				Usage:       "File format / programming language (e.g.: yaml, java, html, ...)",
			},
			&cli.StringFlag{
				Name:    "line-comment-chars",
				Aliases: []string{"lcc"},
				Usage:   "Specifies the line comment character(s) of your data format",
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
				c.String("block-comment-chars-open"),
				c.String("block-comment-chars-close"),
				c.String("data"),
				c.Bool("force"),
				c.String("lang"),
				c.String("line-comment-chars"),
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
