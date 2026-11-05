# petal

Pretty CLI library for C++.

## Features

### CLI input

Support tool name aliases.

Positional arguments are at the start and end at the first flag/option
or at `--`.

**Flags and options**:
Long form: `--output file.txt` or `--output=file.txt`
Short form: `-o file.txt` or `-ofile.txt`
Boolean flags: `--verbose` or `-v`, also negations like `--no-color`
Combined short flags: `-abc`, equals `-a -b -c`
Counted flags: `-vvv` for verbosity levels
Repeatable options: `--include a --include b`

**Sub commands**:
Support nesting commands like `docker container ls`

Built in help command
