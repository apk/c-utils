all : $(HOME)/bin/rdlog $(HOME)/bin/args $(HOME)/bin/xd $(HOME)/bin/cdx $(HOME)/bin/aenv $(HOME)/bin/escat $(HOME)/bin/unsc $(HOME)/bin/sc

$(HOME)/bin/rdlog : rdlog.c
	cc -o $@ $^

$(HOME)/bin/args : args.c
	cc -o $@ $^

$(HOME)/bin/xd : xd.c
	cc -o $@ $^

$(HOME)/bin/cdx : aenv.c
	cc -D CDX -o $@ $^

$(HOME)/bin/aenv : aenv.c
	cc -o $@ $^

$(HOME)/bin/escat : escat.c
	cc -o $@ $^

$(HOME)/bin/unsc : sc.c
	cc -D UN -o $@ $^

$(HOME)/bin/sc : sc.c
	cc -o $@ $^
