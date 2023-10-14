#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
    int p2c[2], c2p[2];
    char buf[10];

    // Create two pipes: one for parent-to-child and one for child-to-parent communication
    if (pipe(p2c) < 0 || pipe(c2p) < 0) {
        fprintf(2, "pingpong: pipe failed\n");
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "pingpong: fork failed\n");
        exit(1);
    }

    if (pid == 0) { // child
        close(p2c[1]);
        close(c2p[0]);
        if (read(p2c[0], buf, sizeof(buf)) <= 0) {
            fprintf(2, "pingpong: child read failed\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());
        write(c2p[1], buf, 1);
        close(p2c[0]);
        close(c2p[1]);
        exit(0);
    } else { // parent
        close(p2c[0]);
        close(c2p[1]);
        write(p2c[1], "p", 1); // send a byte to the child
        if (read(c2p[0], buf, sizeof(buf)) <= 0) {
            fprintf(2, "pingpong: parent read failed\n");
            exit(1);
        }
        printf("%d: received pong\n", getpid());
        close(p2c[1]);
        close(c2p[0]);
        wait(0); // wait for the child to finish
    }
    exit(0);
}

