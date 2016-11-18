all : $(HOME)/bin/rdlog $(HOME)/bin/args $(HOME)/bin/xd $(HOME)/bin/cdx $(HOME)/bin/aenv

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
