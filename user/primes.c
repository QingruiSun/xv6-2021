#include "kernel/types.h"
#include "user/user.h"

void
filter(int oldp[2])
{
  int old_value;
  if (read(oldp[0], &old_value, 4) == 0) {
    close(oldp[0]);
    exit(0);
  }
  printf("prime %d\n", old_value);
  int newp[2];
  pipe(newp);
  if (fork() == 0) {
    close(oldp[0]);
    close(newp[1]);
    filter(newp);
  } else {
    close(newp[0]);
    int new_value;
    while (read(oldp[0], &new_value, 4) > 0) {
      if (new_value % old_value != 0) {
	write(newp[1], &new_value, 4);
      }
    }
    close(oldp[0]);
    close(newp[1]);
    int wait_status;
    wait(&wait_status);
  }
}

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);
  for (int i = 2; i <= 35; ++i) {
    write(p[1], &i, 4);
  }
  close(p[1]);
  if (fork() == 0) {
    filter(p);
  } else {
    close(p[0]);
    int wait_status;
    wait(&wait_status);
  }
  exit(0);
}
