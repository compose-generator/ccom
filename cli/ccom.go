/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

package main

import (
	"ccom/cmd"
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
		Copyright: "© 2021-2022 Marc Auberer",
		Usage:     "Evaluate conditional comment sections of data files.",
		Flags:     cmd.CompileCliFlags,
		Action:    cmd.Compile,
		Commands: []*cli.Command{
			{
				Name:    "compile",
				Aliases: []string{"c"},
				Usage:   "Compiles a source file or a single statement",
				Flags:   cmd.CompileCliFlags,
				Action:  cmd.Compile,
			},
			{
				Name:    "benchmark",
				Aliases: []string{"b"},
				Usage:   "Benchmarks the compiler",
				Flags:   cmd.BenchmarkCliFlags,
				Action:  cmd.Benchmark,
			},
		},
		UseShortOptionHandling: true,
	}

	err := app.Run(os.Args)
	if err != nil {
		log.Fatal(err)
	}
}
