/* -*- mode: C; c-basic-offset: 8; -*- */

#include <stdio.h>
#include <stdlib.h>

dofd (int fd) {
	int z = 0;
	unsigned char buf [16];
	while (1) {
		int i;
		int r = read (fd, buf, sizeof (buf));
		if (r == -1) {
			printf ("Read error\n");
			return z;
		}
		if (r == 0) {
			return z;
		}
		printf ("%5x: ", z);
		for (i = 0; i < 16; i ++) {
			if (i < r) {
				printf (
					"%c%02x",
					i ["    .   .   .   "],
					buf [i]);
			} else {
				printf ("   ");
			}
		}
		printf ("  '");
		for (i = 0; i < 16 && i < r; i ++) {
			unsigned char c = buf [i];
			if (c >= ' ' && c <= '~') {
				printf ("%c", c);
			} else {
				printf (".");
			}
		}
		printf ("'\n");
		z += r;
	}
}

main (int argc, char **argv) {
	int i;
	if (argc > 1) {
		for (i = 1; i < argc; i ++) {
			int fd = open (argv [i], 0);
			if (fd != -1) {
				if (argc > 2) {
					printf ("%s:\n", argv [i]);
				}
				dofd (fd);
				close (fd);
			} else {
				printf ("Cannot open %s\n", argv [i]);
			}
		}
	} else {
		dofd (0);
	}
	return 0;
}
