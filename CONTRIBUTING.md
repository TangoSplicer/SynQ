# Contributing to SynQ

First off, thank you for considering contributing to SynQ! It's people like you that make SynQ such a great tool.

This document provides some guidelines for contributing to SynQ. Please feel free to propose changes to this document in a pull request.

## How to Contribute

There are many ways to contribute to SynQ, and all of them are appreciated. Here are some of the ways you can contribute:

*   **Reporting Bugs:** If you find a bug, please open an issue on our GitHub repository. Please include as much detail as possible, including the version of SynQ you are using, the operating system you are using, and the steps to reproduce the bug.
*   **Suggesting Enhancements:** If you have an idea for a new feature or an enhancement to an existing feature, please open an issue on our GitHub repository. Please include a detailed description of the enhancement and why you think it would be a good addition to SynQ.
*   **Writing Code:** If you want to contribute code to SynQ, please see the "Pull Request Process" section below.
*   **Writing Documentation:** If you want to contribute to the documentation, please see the "Documentation" section below.

## Pull Request Process

1.  Ensure any install or build dependencies are removed before the end of the layer when doing a build.
2.  Increase the version numbers in any examples files and the README.md to the new version that this Pull Request would represent. The versioning scheme we use is [SemVer](http://semver.org/).
3.  You may merge the Pull Request in once you have the sign-off of two other developers, or if you do not have permission to do that, you may request the second reviewer to merge it for you.

## Documentation

The documentation for SynQ is located in the `docs/` directory. The documentation is written in Markdown. If you want to contribute to the documentation, please open a pull request with your changes.

## Development Environment Setup

To set up a development environment for SynQ, you will need to have the following installed:

*   A C++17 compiler (e.g., GCC, Clang)
*   CMake 3.18 or higher
*   Python 3.8 or higher
*   pybind11
*   lcov (for generating coverage reports)

Once you have these dependencies installed, you can build SynQ by running the following commands:

```bash
mkdir build
cd build
cmake ..
make
```

To run the tests, run the following command:

```bash
ctest
```

To generate a coverage report, run the following command:

```bash
./scripts/coverage.sh
```
