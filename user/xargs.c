#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"

#define is_blank(chr) (chr == ' ' || chr == '\t')

int main(int argc, char *argv[]) {
  char input_buffer[2048], ch;
  char *current_position = input_buffer;
  char *command_and_args[MAXARG];
  int arg_count;
  int blanks_count = 0;
  int buffer_offset = 0;

  if (argc <= 1) {
    fprintf(2, "usage: xargs <command> [argv...]\n");
    exit(1);
  }

  for (arg_count = 1; arg_count < argc; arg_count++) {
    command_and_args[arg_count - 1] = argv[arg_count];
  }
  --arg_count;

  while (read(0, &ch, 1) > 0) {
    if (is_blank(ch)) {
      blanks_count++;
      continue;
    }

    if (blanks_count) {
      input_buffer[buffer_offset++] = 0;

      command_and_args[arg_count++] = current_position;
      current_position = input_buffer + buffer_offset;

      blanks_count = 0;
    }

    if (ch != '\n') {
      input_buffer[buffer_offset++] = ch;
    } else {
      command_and_args[arg_count++] = current_position;
      current_position = input_buffer + buffer_offset;

      if (!fork()) {
        exit(exec(command_and_args[0], command_and_args));
      }
      wait(0);

      arg_count = argc - 1;
    }
  }

  exit(0);
}
