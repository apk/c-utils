/* -*- mode: C; c-basic-offset: 8; -*- */

/* run-on-change escat.c test/escat.sh -- sh test/escat.sh
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

static int d_flg = 0;

static int hex (char x) {
	switch (x) {
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'a': case 'A': return 10;
	case 'b': case 'B': return 11;
	case 'c': case 'C': return 12;
	case 'd': case 'D': return 13;
	case 'e': case 'E': return 14;
	case 'f': case 'F': return 15;
	default: return -1;
	}
}

dofd (int fd) {
	char buf [8192];
	int sz = 0;
	while (1) {
		int i;
		int r = read (fd, buf + sz, sizeof (buf) - sz);
		if (r == 0) {
			/* Dump remaining buffer if any. */
			for (i = 0; i < sz; i ++) {
				printf ("%c", buf [i]);
			}
			return 0;
		}
		if (r < 0) {
			perror ("read");
			return 1;
		}
		r += sz;
		for (i = 0; i < r; i ++) {
			char c = buf [i];
			if (d_flg) {
				if (c == '`') {
					if (i + 2 < r) {
						int a = hex (buf [i + 1]);
						int b = hex (buf [i + 2]);
						if (a >= 0 && b >= 0) {
							printf ("%c",
								a * 16 + b);
							i += 2;
							continue;
						}
					} else if (i > 0) {
						/* Too few chars to decode,
						 * and possibly at end of buffer,
						 * pull buffer up, read more, and
						 * try again.
						 */
						break;
					}
				}
				printf ("%c", c);
			} else if ((c >= ' ' && c <= '~' && c != '`') ||
			    c == '\n' || c == '\r' || c == '\t')
			{
				printf ("%c", c);
			} else {
				printf ("`%02x", c & 255);
			}
		}
		sz = r - i;
		if (sz > 0) {
			/* Won't ever move more that two bytes. */
			memmove (buf, buf + i, sz);
		}
	}
}

main (int argc, char **argv) {
	int i;
	for (i = 1; i < argc; i ++) {
		char *a = argv [i];
		char *p = a + 1;
		if (*a != '-') break;
		for ( ; *p; p ++) {
			switch (*p) {
			case 'd':
				d_flg = 1;
				break;
			default:
				fprintf (stderr, "Bad arg '%s'\n", a);
				exit (1);
				break;
			}
		}
	}
	if (i == argc) {
		dofd (0);
	} else {
		for ( ; i < argc; i ++) {
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
