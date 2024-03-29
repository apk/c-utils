/* -*- mode: C; c-basic-offset: 8; -*- */

#include <stdio.h>

int main (int argc, char **argv) {
	int multiline = 0;
	int always = 0;
	int hide = 0;
	int i;
	int nextsep = 0;
	if (argc > 1) {
		char *pc = argv [1];
		if (*pc == '-') {
			char *h = pc + 1;
			while (*h) {
				switch (*h ++) {
				case 'q':
					always = 1;
					break;
				case 'm':
					multiline = 1;
					break;
				case 'h':
					hide = 1;
					break;
				}
			}
		}
	}
	for (i = 0; i < argc; i ++) {
		char *pc = argv [i];
		/* hide can only be true when argv[1]
		 * is the hide option, so don't check
		 * again for the opt.
		 */
		if (i == 1 && hide) continue;
		if (nextsep) {
			putchar (multiline ? '\n' : ' ');
		}
		if (!always) {
			for ( ; *pc; pc ++) {
				int c = 0xff & *pc;
				if ((c >= 'a' && c <= 'z') ||
				    (c >= 'A' && c <= 'Z') ||
				    (c >= '0' && c <= '9') ||
				    c == '-' || c == '=' ||
				    c == '.' || c == '_' ||
				    c == ',' || c == '+' ||
				    c == ':' || c == '@' ||
				    c == '/') {
				} else {
					break;
				}
			}
		}
		if (*pc || pc == argv [i]) {
			putchar ('\"');
			for (pc = argv [i]; *pc; pc ++) {
				int c = 0xff & *pc;
				switch (c) {
				case '\"':
				case '\'':
				case '\\':
					printf ("\\%c", c);
					break;
				case '\n':
					printf ("\\n");
					break;
				case '\t':
					printf ("\\t");
					break;
				default:
					if (c >= ' ' && c <= '~') {
						putchar (c);
					} else {
						printf ("\\%03o", c);
					}
					break;
				}
			}
			putchar ('\"');
		} else {
			for (pc = argv [i]; *pc; pc ++) {
				int c = 0xff & *pc;
				putchar (c);
			}
		}
		nextsep = 1;
	}
	if (nextsep) {
		putchar ('\n');
	}
	return 0;
}
