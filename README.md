# C utils

This is a set of unix utility program that I wrote because I found them helpful.

## `rdlog`

Like `tail -f`, except it always outputs the entire file, and it does also follow
rename and recreation of the file.

## `args`

Shows its arguments, printing them C-escaped when characters in the values
are sufficiently strange.

## `xd`

Dump each file given as argument as a hex and ascii dump, in the style
of early debuggers and monitors. Read stdin if no arguments are given.
