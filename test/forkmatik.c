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
      pid_t p = fork ();
      if (p == 0) {
	printf ("run %d, %d\n", getpid (), getppid ());
	sleep (c);
	printf ("exit(%d) %d, %d\n", c, getpid (), getppid ());
	exit (c);
      }
      sleep (1);
      if (fork () == 0) {
        sleep (2);
        switch (c) {
          case 3: kill (p, SIGINT);
          case 4: kill (p, SIGTERM);
        }
        exit (0);
      }
      exit (10+c);
    }
    sleep (4);
  }
}

/*

  cd ..
  cat >Dockerfile <<EOF
  centos:7
  COPY aenv forkmatik /data/
  WORKDIR /data
  CMD ["/data/aenv","--reaper-fork","/data/forkmatik"]
  EOF
  for i in test/forkmatik.c aenv.c; do gcc -o `basename $i .c` $i; done
  docker build -t asdf . && docker run asdf

*/
