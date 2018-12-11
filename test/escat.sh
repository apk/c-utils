#!/bin/sh

cd "`dirname "$0"`"

escat=$HOME/bin/escat

make -C .. $escat

esc () {
    r="`echo "$1" | $escat -d`"
    if test x"$r" != x"$2"; then
	echo "Fail: $r isn't $2 ($1)"
	exit 1
    else
	echo "Ok: $r ($1)"
    fi
}

rt () {
    r="`echo "$1" | $escat -d | $escat`"
    if test x"$r" != x"$2"; then
	echo "Fail: $r isn't $2 ($1)"
	exit 1
    else
	echo "Ok: $r ($1)"
    fi
}

enc () {
    r="`echo "$1" | $escat`"
    if test x"$r" != x"$2"; then
	echo "Fail: $r isn't $2 ($1)"
	exit 1
    else
	echo "Ok: $r ($1)"
    fi
}

esc 'ab`63' 'abc'
esc 'ab`6ft' 'abot'
esc 'ab`6Ft' 'abot'
esc 'ab`xy' 'ab`xy'
esc 'ab`6x' 'ab`6x'
esc 'ab`' 'ab`'

rt 'ab`05' 'ab`05'
rt 'ab`20-' 'ab -'
rt 'ab`ff-' 'ab`ff-'
rt 'ab`0a-' 'ab
-'

enc 'ab`-' 'ab`60-'

dd if=/dev/urandom bs=1M count=4 >.rnd

# You'd guess this will hit the ` at end of buffer
# (within a file) condition at least once.
if cat .rnd | escat | escat -d | cmp - .rnd; then
    echo 'rand ok'
else
    echo 'rand fail'
    exit 1
fi
rm -f .rnd

echo All ok
