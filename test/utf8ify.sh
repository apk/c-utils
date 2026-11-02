#!/bin/sh

cd "`dirname "$0"`"

utf8ify=$HOME/bin/utf8ify

make -C .. $utf8ify

t () {
    r="`printf "$1" | $utf8ify | od -An -tx1 | tr -d ' \n'`"
    if test x"$r" != x"$2"; then
	echo "Fail: $r isn't $2 ($1)"
	exit 1
    else
	echo "Ok: $r ($1)"
    fi
}

# valid utf-8 passes through unchanged
t '\303\251' 'c3a9'

# lone latin-1 e-acute byte -> repaired to utf-8
t '\351' 'c3a9'

# cp1252 0x93 (left double quote) -> U+201C
t '\223' 'e2809c'

# cp1252 undefined slot 0x81 -> falls back to raw latin-1 (c1 control)
t '\201' 'c281'

# overlong 2-byte encoding of nul (c0 80): each byte repaired on its own
t '\300\200' 'c380e282ac'

# truncated sequence at eof: lone c3 -> latin-1 Ã
t '\303' 'c383'

# surrogate half encoded as utf-8 (u+d800): invalid, bytes repaired individually
t '\355\240\200' 'c3adc2a0e282ac'

echo "=== multi-file and - handling ==="

echo hello >.a
echo world >.b

r="`$utf8ify .a .b`"
if test x"$r" = x"`printf 'hello\nworld\n'`"; then
    echo "Ok: multi-file concat"
else
    echo "Fail: multi-file concat gave: $r"
    exit 1
fi

r="`echo from-stdin | $utf8ify .a -`"
if test x"$r" = x"`printf 'hello\nfrom-stdin\n'`"; then
    echo "Ok: dash means stdin"
else
    echo "Fail: dash-stdin gave: $r"
    exit 1
fi

if $utf8ify .a .nonexistent .b >/dev/null 2>/dev/null; then
    echo "Fail: missing file should give nonzero exit"
    exit 1
else
    echo "Ok: missing file gives nonzero exit"
fi

r="`$utf8ify .a .nonexistent .b 2>/dev/null`"
if test x"$r" = x"`printf 'hello\nworld\n'`"; then
    echo "Ok: missing file is skipped, others still processed"
else
    echo "Fail: missing-file-skip gave: $r"
    exit 1
fi

rm -f .a .b

echo All ok
