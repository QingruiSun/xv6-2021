#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2];
  int p2[2];
  pipe(p1);
  pipe(p2);
  if (fork() == 0) {
    char c;
    int cpid = getpid();
    close(p1[1]);
    close(p2[0]);
    read(p1[0], &c, 1);
    close(p1[0]);
    fprintf(1, "%d: received ping\n", cpid);
    write(p2[1], &c, 1);
    close(p2[1]);
    exit(0);
  } else {
    char c = 'a';
    int ppid = getpid();
    close(p1[0]);
    close(p2[1]);
    write(p1[1], &c, 1);
    close(p1[1]);
    read(p2[0], &c, 1);
    fprintf(1, "%d: received pong\n", ppid);
    exit(0);
  }
}
