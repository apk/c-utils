/* -*- mode: C; c-basic-offset: 8; -*- */

#include <stdio.h>

main (int iArgc, char **ppszArgv) {
	int i;
	for (i = 0; i < iArgc; i ++) {
		char *pc;
		for (pc = ppszArgv [i]; *pc; pc ++) {
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
		if (*pc || pc == ppszArgv [i]) {
			putchar ('\"');
			for (pc = ppszArgv [i]; *pc; pc ++) {
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
			for (pc = ppszArgv [i]; *pc; pc ++) {
				int c = 0xff & *pc;
				putchar (c);
			}
		}
		putchar (i == iArgc - 1 ? '\n' : ' ');
	}
	return 0;
}
