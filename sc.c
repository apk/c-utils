#include <stdio.h>

int main () {
  int b = 0;
  int c;
  while ((c = getchar ()) != EOF) {
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
  return 0;
}
