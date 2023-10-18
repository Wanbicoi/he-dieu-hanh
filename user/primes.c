#include "kernel/types.h"
#include "user/user.h"

int seive(int fd_read) {
  int prime, num;
  if (read(fd_read, &prime, sizeof(int)) != sizeof(int)) {
    close(fd_read);
    exit(0);
  }
  printf("prime: %d \n", prime);

  int fds[2];
  pipe(fds);
  if (fork() == 0) {
    close(fd_read);
    close(fds[1]);
    seive(fds[0]);
  } else {
    close(fds[0]);
    while (read(fd_read, &num, sizeof(int))) {
      if (num % prime != 0) {
        write(fds[1], &num, sizeof(int));
      }
    }
    close(fds[1]);
    close(fd_read);
    wait(0);
  }
  exit(0);
}

int main(void) {
  int fds[2];

  pipe(fds);
  if (fork() == 0) {
    close(fds[1]);
    seive(fds[0]);
  } else {
    close(fds[0]);

    for (int i = 2; i <= 35; i++)
      write(fds[1], &i, sizeof(int));

    close(fds[1]);
    wait(0);
  }

  exit(0);
}
