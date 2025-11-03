#include "hdr.h"

static void dofd (FILE *fd) {
  int b = 0;
  int c;
  while ((c = getc (fd)) != EOF) {
    if (c >= ' ' && c <= '~') {
#ifdef UN
      b=(c - ' ')-b;
      b+=95;
      b%=95;
      putchar(' '+b);
      b=c-' ';
#else
      b+=(c - ' ');
      b%=95;
      putchar(' '+b);
#endif
    } else {
      putchar (c);
    }
  }
}

int main (int argc, char **argv) {
	int i;
	for (i = 1; i < argc; i ++) {
		char *a = argv [i];
		char *p = a + 1;
		if (*a != '-') break;
		fprintf (stderr, "Bad opt '%s'\n", a);
		exit (1);
	}
	if (i == argc) {
		dofd (stdin);
	} else {
		for ( ; i < argc; i ++) {
			FILE *fd = fopen (argv [i], "r");
			if (!fd) {
				perror ("open");
				exit (1);
			}
			dofd (fd);
			fclose (fd);
		}
	}
	return 0;
}
