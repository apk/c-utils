all : $(HOME)/bin/rdlog $(HOME)/bin/args $(HOME)/bin/xd $(HOME)/bin/cdx $(HOME)/bin/aenv $(HOME)/bin/escat $(HOME)/bin/unsc $(HOME)/bin/sc

$(HOME)/bin/rdlog : rdlog.c hdr.h
	cc -o $@ $^

$(HOME)/bin/args : args.c hdr.h
	cc -o $@ $^

$(HOME)/bin/xd : xd.c hdr.h
	cc -o $@ $^

$(HOME)/bin/cdx : aenv.c hdr.h
	cc -D CDX -o $@ $^

$(HOME)/bin/aenv : aenv.c hdr.h
	cc -o $@ $^

$(HOME)/bin/escat : escat.c hdr.h
	cc -o $@ $^

$(HOME)/bin/unsc : sc.c
	cc -D UN -o $@ $^

$(HOME)/bin/sc : sc.c
	cc -o $@ $^
