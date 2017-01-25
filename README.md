# C utils

This is a set of unix utility program that I wrote because I found them helpful.

## `rdlog`

Like `tail -f`, except it always outputs the entire file, and it does also follow
rename and recreation of the file.

## `aenv`

Similar to `env`, but having a different option set:

* `-C arg` changes into the given directory.
* `-v` shows the currently remaining arguments.
* `-wN` sleeps for N seconds.
* `-X` replaces every sequence `X<hex><hex>` in all following
  arguments given by the character value `<hex><hex>. This is
  a hack for some ssh versions that `s/=/ /` on `ProxyCommand`.
* `-Xc` similar, but use the given char instead of `X`.
* `NAME=value` sets the environment variable `NAME` like `env` does.

The first thing that is not one of the above is taken as the command
to execute, with the following arguments as parameters.

## `cdx`

Like `aenv`, except that the first argument that would otherwise be
taken as the command names a directory to change to, and processing
continues to the next such argument. So `cdx dir git ...` is the
same as `git -C dir ...`, and `cdx dir GIT_COMMITTER_NAME='John Doe' git ...`
will pass that environment variable to git.

It's somewhat stupid that the builtin `cd` or various shells doesn't do this.

## `args`

Shows its arguments, printing them C-escaped when characters in the values
are sufficiently strange.

## `xd`

Dump each file given as argument as a hex and ascii dump, in the style
of early debuggers and monitors. Read stdin if no arguments are given.

## `escat`

Like cat, but escape special characters, and backticks, by a backtick
and the two hex digits representing the character. Special characters
are everything but printable asccii characters, newlines, tabs, and
carriage returns.
