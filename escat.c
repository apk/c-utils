/* -*- mode: C; c-basic-offset: 8; -*- */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

dofd (int fd) {
	char tbuf [80];
	char buf [256];
	int l = 0;
	while (1) {
		int r;
		int i;
		r = read (fd, buf, sizeof (buf));
		if (r == 0) return 0;
		if (r < 0) {
			perror ("read");
			return 1;
		}
		for (i = 0; i < r; i ++) {
			char c = buf [i];
			if ((c >= ' ' && c <= '~' && c != '`') ||
				c == '\n' || c == '\r' || c == '\t')
			{
				printf ("%c", c);
			} else {
				printf ("`%02x", c & 255);
			}
		}
	}
}

main (int argc, char **argv) {
	int i;
	if (argc <= 1) {
		dofd (0);
	} else {
		for (i = 1; i < argc; i ++) {
			int fd = open (argv [i], 0);
			if (fd == -1) {
				perror ("open");
				exit (1);
			}
			dofd (fd);
			close (fd);
		}
	}
	return 0;
}
