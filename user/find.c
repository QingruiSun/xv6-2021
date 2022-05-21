#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void
find(char *path, char *name) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: can't stat file %s\n", path);
    close(fd);
    return;
  }
  
  if (st.type != T_DIR) {
    close(fd);
    return;
  }

  strcpy(buf, path);
  p = buf + strlen(buf);
  while(read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0) {
      continue;
    }
    if (strcmp(de.name, ".") == 0) {
      continue;
    }
    if (strcmp(de.name, "..") == 0) {
      continue;
    }
    *p++ = '/';
    strcpy(p, de.name);
    if (strcmp(de.name, name) == 0) {
      fprintf(1, "%s\n", buf);
    } else {
      find(buf, name);
    }
    p--;
    memset(p, 0, strlen(p));
  }
  close(fd); 
}

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    fprintf(2, "Usage: find directory name...\n");
    exit(1);
  }

  find(argv[1], argv[2]);

  exit(0);
}
