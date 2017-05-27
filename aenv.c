/* -*- mode: C; c-basic-offset: 8; -*- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>

int hexof (int c) {
	switch (c) {
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
	case 'A': case 'a': return 10;
	case 'B': case 'b': return 11;
	case 'C': case 'c': return 12;
	case 'D': case 'd': return 13;
	case 'E': case 'e': return 14;
	case 'F': case 'f': return 15;
	default: return -1;
	}
}

main (int argc, char **argv) {
	int i = 1;
#ifdef CDX
	int wantcd = 1;
#endif
	for ( ; i < argc; i ++) {
		int del;
		char *q;
		if (!strcmp (argv [i], "-C")) {
			if (i + 1 < argc) {
				if (chdir (argv [++ i])) {
					fprintf (stderr, "Cannot chdir to %s\n", argv [i]);
					return 1;
				}
				continue;
			}
			fprintf (stderr, "No arg for -C\n");
			return 1;
		} else if (!strcmp (argv [i], "-O")) {
			if (i + 1 < argc) {
				int fd = open (argv [++ i], O_WRONLY | O_CREAT | O_TRUNC, 0666);
				if (fd == -1) {
					fprintf (stderr, "Cannot open %s for writing\n", argv [i]);
					return 1;
				}
				if (dup2 (fd, 1) == -1) {
					fprintf (stderr, "Dup failed for %s\n", argv [i]);
					return 1;
				}
				close (fd);
				continue;
			}
			fprintf (stderr, "No arg for -O\n");
			return 1;
		} else if (!strcmp (argv [i], "-I")) {
			if (i + 1 < argc) {
				int fd = open (argv [++ i], 0);
				if (fd == -1) {
					fprintf (stderr, "Cannot open %s for reading\n", argv [i]);
					return 1;
				}
				if (dup2 (fd, 0) == -1) {
					fprintf (stderr, "Dup failed for %s\n", argv [i]);
					return 1;
				}
				close (fd);
				continue;
			}
			fprintf (stderr, "No arg for -I\n");
			return 1;
		} else if (!strcmp (argv [i], "--pwnam")) {
			if (i + 1 < argc) {
				char *u = argv [++ i];
				struct passwd *p = getpwnam (u);
				if (!p) {
					fprintf (stderr, "No passwd entry for %s\n", u);
					return 3;
				}
				if (setregid (p->pw_gid, p->pw_gid)) {
					fprintf (stderr, "setgid failed\n");
					return 4;
				}
				if (setreuid (p->pw_uid, p->pw_uid)) {
					fprintf (stderr, "setuid failed\n");
					return 4;
				}
				continue;
			}
			fprintf (stderr, "No arg for --pwnam\n");
			return 1;
		} else if (argv [i] [0] == '-' &&
			   argv [i] [1] == 'v' &&
			   argv [i] [2] == 0)
		{
			int j;
			char *s = "args: ";
			for (j = i + 1; argv [j]; j ++) {
				printf ("%s%s", s, argv [j]);
				s = " ";
			}
			printf ("\n");
		} else if (argv [i] [0] == '-' &&
			   argv [i] [1] == 'w' &&
			   ((del = strtol (argv [i] + 2, &q, 10)), !*q)&&
			   del > 0)
		{
			sleep (del);
		} else if (argv [i] [0] == '-' &&
			   argv [i] [1] == 'X' &&
			   (argv [i] [2] == 0 ||
			    argv [i] [3] == 0))
		{
			/* This was introduced as a way around some ssh
			 * releases that thought it a good idea to to an
			 * s/=/ / on the ProxyCommand...
			 * -> aenv -X cmd argX3dvalue
			 */
			char x = argv [i] [2];
			int j;
			if (!x) x = 'X';
			for (j = i + 1; j < argc; j ++) {
				char *s = argv [j];
				char *t = s;
				while (*s) {
					int d, e;
					if (*s == x &&
					    (d = hexof (s [1])) != -1 &&
					    (e = hexof (s [2])) != -1)
					{
						*t ++ = 16 * d + e;
						s += 3;
					} else {
						*t ++ = *s ++;
					}
				}
				*t = 0;
			}
		} else if (strcmp (argv [i], "--fork") == 0) {
			int r = fork ();
			if (r == -1) {
				fprintf (stderr, "Cannot fork\n");
				return 1;
			}
			if (r != 0) {
				return 0;
			}
		} else if (strchr (argv [i], '=')) {
			if (putenv (argv [i])) {
				fprintf (stderr, "No memory\n");
				return 1;
			}
#ifdef CDX
		} else if (wantcd) {
			wantcd = 0;
			if (chdir (argv [i])) {
				fprintf (stderr, "Cannot chdir to %s\n", argv [i]);
				return 1;
			}
#endif
		} else {
			break;
		}
	}
	if (i < argc) {
		execvp (argv [i], argv + i);
		fprintf (stderr, "Exec (%s) failed...\n", argv [i]);
#ifdef CDX
	} else if (wantcd) {
		fprintf (stderr, "Missing dir argument\n");
		return 1;
#endif
	} else {
		char *shell = getenv ("SHELL");
		char *a [2];
		if (!shell) shell = "/bin/sh";
		a [0] = shell;
		a [1] = 0;
		execvp (a [0], a);
		fprintf (stderr, "Exec (%s) failed...\n", shell);
	}
	return 1;
}
