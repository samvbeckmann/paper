# Paper
Paper is an open-source compiler for a subset of the Pascal language.

It was created for CS 4013 at the University of Tulsa in Fall 2016 by Sam Beckmann.

## Assumptions
These are some assumptions made by the compiler. Not following these with
Pascal code may break things.
- Lines will be no longer than **72** characters, including null terminators.
- variable names and number literals will be no longer **29** characters.
    - Only 10 or fewer characters are supported for compilation, but more than 29 characters may cause issues with the compiler.
