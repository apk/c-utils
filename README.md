# C utils

This is a set of unix utility program that I wrote because I found them helpful.

## `aenv`

Similar to `env`, but extended to a swiss army knife,
to avoid shell processes or shim shells altogether. Options:

* `-C arg` changes into the given directory.
* `-v` shows the currently remaining arguments.
* `-wN` sleeps for N seconds.
* `NAME=value` sets the environment variable `NAME` like `env` does.
* `-S NAME values` sets the environment variable `NAME` to `value`.
* `-U NAME` unsets the environment variable `NAME`.
* `-I file` redirects stdin to read from the given file.
* `-O file` redirects stdout to write to the given file.
* `-X` replaces every sequence `X<hex><hex>` in all following
  arguments given by the character value `<hex><hex>`. This is
  a hack for some ssh versions that `s/=/ /` on `ProxyCommand`.
* `-Xc` similar, but use the given char instead of `X`.
* `--pwnam user` change (primary) group and user id to that of
  the given user. Requires sufficient privileges.
* `--su user`: Like `--pwnam`, but also sets `HOME`, `USER`,
  and `LOGNAME` accordingly.
* `--fork` goes into background by forking and exiting in the
  foreground process.
* `--reaper-fork` continues option and argument processing in a
  forked process. The main process waits for the subprocess to
  terminate, and also waits for other children, and passes
  `SIGINT` and `SIGTERM` on to the subprocess (and only to
  that pid). Useful as a docker root process in case of zombies.
  Each process thus reaped is logged on stderr, with its pid,
  its exit code if nonzero, and the killing signal if applicable.
* `-AN` sets `alarm()` in N seconds, thereby providing a hard
  timeout for the command finally being executed (caveats apply).

Options are processed in order they are given.

The first thing that is not one of the above is taken as the command
to execute, with the following arguments as parameters.

## `cdx`

Like `aenv`, except that the first argument that would otherwise be
taken as the command names a directory to change to, and processing
continues to the next such argument. So `cdx dir git ...` is the
same as `git -C dir ...`, and `cdx dir GIT_COMMITTER_NAME='John Doe' git ...`
will pass that environment variable to git.

It's somewhat stupid that the builtin `cd` or various shells doesn't do this.

## `rdlog`

Like `tail -f`, except it always outputs the entire file, and it does also follow
rename and recreation of the file.

## `args`

Shows its arguments, printing them C-escaped when characters in the values
are sufficiently strange. Takes a single option argument as its first
argument if that starts with a minus. If a `q` is there all output is
quoted, not only when needed. A `m` enables multiline output - there
will be a newline after each argument instead of everything being on
one line. A `h` causes the option argument itself to be omitted.

## `xd`

Dump each file given as argument as a hex and ascii dump, in the style
of early debuggers and monitors. Read stdin if no arguments are given.

## `escat`

Like cat, but escape special characters, and backticks, by a backtick
and the two hex digits representing the character. Special characters
are everything but printable asccii characters, newlines, tabs, and
carriage returns.
