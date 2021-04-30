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
			&cli.StringFlag{Name: "comment-char", Aliases: []string{"c"}, DefaultText: "#", Usage: "Sets the comment character(s) of your language"},
			&cli.StringFlag{Name: "data", Aliases: []string{"d"}, DefaultText: "{}", Usage: "JSON string or path to JSON file, which holds the evaluation work data"},
			&cli.BoolFlag{Name: "preserve-comments-on-false", Aliases: []string{"p"}, Usage: "Do not delete conditional section after evaluating. Default is to delete them"},
		},
		Action: func(c *cli.Context) error {
			processInput(c.Args().Get(0), c.String("data"), c.String("comment-char"), c.Bool("preserve-comments-on-false"))
			return nil
		},
		UseShortOptionHandling: true,
	}

	err := app.Run(os.Args)
	if err != nil {
		log.Fatal(err)
	}
}
