#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


int main(void) {
    int pipefd[2];
    if (pipe(pipefd)) {
        fprintf(stderr, "Init pipe error.\n");
        return 1;
    }

    // Create a child process.
    pid_t pid = fork();
    if (pid == -1) {
        fprintf(stderr, "Fork child process error.\n");
        return 1;
    }

    if (pid == 0) {
        // This is the child process.
        // Read buffer from pipe, one character at a time.
        char buf;
        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);
        close(pipefd[0]);

        _exit(EXIT_SUCCESS);
    } else {
        // This is the parent process.
        // Write string to pipe, assume write success
        const char* str = "A random string to read from pipe.\n";
        write(pipefd[1], str, strlen(str));
        close(pipefd[1]);

        exit(EXIT_SUCCESS);
    }
}
