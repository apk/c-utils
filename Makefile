all : $(HOME)/bin/rdlog $(HOME)/bin/args

$(HOME)/bin/rdlog : rdlog.c
	cc -o $@ $^

$(HOME)/bin/args : args.c
	cc -o $@ $^
