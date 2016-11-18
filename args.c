#include <stdio.h>

main (int iArgc, char **ppszArgv) {
	int i;
	for (i = 0; i < iArgc; i ++) {
		char *pc;
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
		putchar (i == iArgc - 1 ? '\n' : ' ');
	}
	return 0;
}
