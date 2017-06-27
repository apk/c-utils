#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

main () {
  
  int c = 0;
  signal (SIGCHLD, SIG_IGN);
  while (c < 10) {
    c ++;
    if (fork () == 0) {
      if (fork () == 0) {
	printf ("run %d, %d\n", getpid (), getppid ());
	sleep (c);
	printf ("exit(%d) %d, %d\n", c, getpid (), getppid ());
	exit (c);
      }
      sleep (1);
      exit (10+c);
    }
    sleep (4);
  }
}
