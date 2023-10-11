#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int sleep_time;

  if (argc < 2) {
    exit(1);
  }
  sleep_time = atoi(argv[1]);

  sleep(sleep_time);

  exit(0);
}
