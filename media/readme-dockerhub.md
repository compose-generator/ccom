<p align="center">
  <img alt="CCom Logo" src="https://github.com/compose-generator/ccom/raw/main/media/logo-wide.png" height="280" />
  <h3 align="center">CCom language</h3>
  <p align="center">Compiler for the CCom (Conditional Comments) language.</p>
  <p align="center">
    <a target="_blank" href="https://github.com/compose-generator/ccom/releases/latest"><img src="https://img.shields.io/github/v/release/compose-generator/ccom?include_prereleases"></a>
    <a target="_blank" href="https://hub.docker.com/r/chillibits/ccom"><img src="https://img.shields.io/docker/pulls/chillibits/ccom"></a>
    <a target="_blank" href="https://github.com/compose-generator/ccom/tree/main/.github/workflows/ci-go.yml"><img src="https://github.com/compose-generator/ccom/actions/workflows/ci-go.yml/badge.svg"></a>
	<a target="_blank" href="https://github.com/compose-generator/ccom/tree/main/.github/workflows/ci-cpp.yml"><img src="https://github.com/compose-generator/ccom/actions/workflows/ci-cpp.yml/badge.svg"></a>
	<a target="_blank" href="https://github.com/compose-generator/ccom/tree/main/.github/workflows/ci-java.yml"><img src="https://github.com/compose-generator/ccom/actions/workflows/ci-java.yml/badge.svg"></a>    
	<a target="_blank" href="https://github.com/compose-generator/ccom/tree/main/.github/workflows/codeql-analysis.yml"><img src="https://github.com/compose-generator/ccom/actions/workflows/codeql-analysis.yml/badge.svg"></a>
    <a target="_blank" href="https://goreportcard.com/report/github.com/compose-generator/ccom"><img src="https://goreportcard.com/badge/github.com/compose-generator/ccom"></a>
    <a target="_blank" href="https://makeapullrequest.com"><img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg"></a>
    <a target="_blank" href="./LICENSE.md"><img src="https://img.shields.io/github/license/compose-generator/ccom"></a>
  </p>
</p>

*Note: This language is part of the [Compose Generator](https://github.com/compose-generator/compose-generator) project, but also can be used independently.*

## Documentation
Please visit the documentation at [ccom.compose-generator.com](https://ccom.compose-generator.com).

## Usage
You can use CCom by directly installing it on your Docker host system or by generating your compose file with the CCom Docker container on the fly.

### Install CCom
For installation instructions for Linux, Windows, etc., please visit the [installation guide](https://ccom.compose-generator.com/install/linux).

## QuickStart with Docker
**Linux:**
```sh
$ docker run --rm -it -v $(pwd):/ccom/out chillibits/ccom
```

**Windows:**
```sh
$ docker run --rm -it -v ${pwd}:/ccom/out chillibits/ccom
```
*Note: This command does not work with Windows CMD command line. Please use Windows PowerShell instead.*

## Contribute otherwise to the project
If you want to contribute to this project, please ensure you comply with the [contribution guidelines](https://github.com/compose-generator/ccom/blob/main/CONTRIBUTING.md).

© Marc Auberer 2021-2023