#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

void 
exec_help(char *argv[MAXARG])
{
  int pid;
  if ((pid = fork()) == 0) { // child process
    exec(argv[0], argv);
  } else {
    int wait_status;
    wait(&wait_status);
  } 
}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
      fprintf(2, "Usage: xargs command ....\n");
      exit(1);
    }
    int max_length = 64;
    char exec_argv[MAXARG][max_length];
    char *pass[MAXARG];
    for (int i = 0; i < MAXARG; ++i) {
      pass[i] = exec_argv[i];
    }
    for (int i = 1; i < argc; ++i) {
      strcpy(&exec_argv[i - 1][0], argv[i]);
    }
    char c;
    int exec_argc = argc - 1;
    int index = 0;
    while (read(0, &c, 1) > 0) {
      if (c == '\n') {
	exec_argv[exec_argc][index] = '\0';
	exec_argc++;
	pass[exec_argc] = 0;
        exec_help(pass);
	exec_argc = argc - 1;
	index = 0;
      } else if (c == ' '){
        exec_argv[exec_argc][index] = '\0';
	exec_argc++;
	index = 0;
      } else {
        exec_argv[exec_argc][index] = c;
	index++;
      }
    }
    exit(0);
}


