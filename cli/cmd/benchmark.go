package cmd

import (
	"ccom/util"
	"strconv"
	"time"

	"github.com/urfave/cli/v2"
)

// BenchmarkCliFlags are the cli flags of the benchmark command
var BenchmarkCliFlags = []cli.Flag{
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
	&cli.IntFlag{
		Name:    "number",
		Aliases: []string{"n"},
		Usage:   "Specifies the number of benchmark runs",
	},
	&cli.BoolFlag{
		Name:    "silent",
		Aliases: []string{"s"},
		Usage:   "Only print raw compiler output and no debug output",
	},
}

// Benchmark executes a CCom benchmark
func Benchmark(c *cli.Context) error {
	// Extract flags
	benchmarkRuns := c.Int("number")

	fileInput := c.Args().Get(0)
	compiler := /*c.String("compiler")*/ "cpp"
	comBlockIdenOpen := c.String("block-comment-iden-open")
	comBlockIdenClose := c.String("block-comment-iden-close")
	comLineIden := c.String("line-comment-iden")
	dataInput := c.String("data")
	lang := c.String("lang")
	modeSingle := c.Bool("mode-single")
	silentFlag := c.Bool("silent")

	// Analyze correctness of inputs
	if !silentFlag {
		util.P("Analyzing inputs ... ")
	}
	util.EnsureInput(&fileInput, &dataInput, &compiler, lang, &comLineIden, &comBlockIdenOpen, &comBlockIdenClose, modeSingle)
	if !silentFlag {
		util.Done()
	}

	overallRuntime := 0
	for i := 1; i <= benchmarkRuns; i++ {
		start := time.Now()
		util.RunCompilerExecutable(compiler, modeSingle, fileInput, dataInput, comLineIden, comBlockIdenOpen, comBlockIdenClose)
		duration := time.Since(start).Milliseconds()
		overallRuntime += int(duration)
		util.Pl(strconv.Itoa(i) + ". run: " + strconv.Itoa(int(duration)) + " ms")
	}
	util.Pel()
	util.Info("Overall runtime: " + strconv.Itoa(overallRuntime) + " ms")
	util.Info("Average runtime: " + strconv.Itoa(overallRuntime/benchmarkRuns) + " ms")

	return nil
}
