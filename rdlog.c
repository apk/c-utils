/* -*- mode: C; c-basic-offset: 8; -*- */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

main (int argc, char **argv) {
        char *fnam;
        int fd;
        int l;
        char buf [1024];

        if (argc < 2) {
		fprintf (stderr, "Usage: rdlog file\n");
		exit (1);
	}
        fnam = argv [1];

        fd = open (fnam, 0);
        if (fd < 0) {
                printf ("Cannot open %s\n", fnam);
                exit (1);
        }

        for (l = 0; ; ) {
                struct stat statbuf, stat2;
                int r, nl, fd2;
                usleep (500000);
                r = fstat (fd, &statbuf);
                if (r == -1) {
                        printf ("fstat failed\n");
                        exit (1);
                }
                nl = statbuf.st_size;
                while (l < nl) {
                        int s = nl - l;
                        int r = read (fd, buf, s < sizeof (buf)
                                                 ? s
                                                 : sizeof (buf));
                        if (r < 0) {
                                printf ("read error\n");
                                exit (1);
                        }
                        if (r == 0) {
                                break;
                        }
                        write (1, buf, r);
                        l += r;
                }
                fd2 = open (fnam, 0);
                if (fd2 < 0) continue;
                r = fstat (fd2, &stat2);
                if (r < 0) {
                        close (fd2);
                        continue;
                }
                if (statbuf.st_ino != stat2.st_ino) {
                        close (fd);
                        fd = fd2;
                        l = 0;
                        continue;
                }
                close (fd2);
        }
}
