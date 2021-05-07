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
				Name:    "line-comment-chars",
				Aliases: []string{"lcc"},
				Usage:   "Specifies the line comment character(s) of your data format",
			},
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
			&cli.StringFlag{
				Name:        "lang",
				Aliases:     []string{"l"},
				DefaultText: "yaml",
				Usage:       "File format / programming language (e.g.: yaml, java, html, ...)",
			},
			&cli.StringFlag{
				Name:        "mode",
				Aliases:     []string{"m"},
				DefaultText: "file",
				Usage:       "Whole file with string output or single statement list with boolean output (file / single)",
			},
			&cli.BoolFlag{
				Name:    "preserve-comments-on-false",
				Aliases: []string{"p"},
				Usage:   "Do not delete conditional section after evaluating. Default is to delete them",
			},
		},
		Action: func(c *cli.Context) error {
			processInput(
				c.Args().Get(0),
				c.String("data"),
				c.String("lang"),
				c.String("mode"),
				c.String("line-comment-chars"),
				c.String("block-comment-chars-open"),
				c.String("block-comment-chars-close"),
				c.Bool("preserve-comments-on-false"),
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
