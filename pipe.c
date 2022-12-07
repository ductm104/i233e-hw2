/*
    This program creates a pipe and writes a string to the write end of the
    pipe.  It then reads the string from the read end of the pipe and prints it
    to the screen. This demonstrates the effect of the pipe system call, which
    allows data to be passed between two endpoints in the same process or in
    different processes.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>


int main(void) {
    // Create a pipe.
    int pipefd[2];
    if (pipe(pipefd)) {
        fprintf(stderr, "Init pipe error.\n");
        return 1;
    }

    // Write a string to the write-end of the pipe.
    const char* str = "Hello from the pipe!";
    const int N = strlen(str);
    ssize_t bytes_written = write(pipefd[1], str, N);

    // Checking write function output
    if (bytes_written == -1) {
        fprintf(stderr, "Write to pipe error.\n");
        return 1;
    }
    
    if (bytes_written < N) {
        fprintf(stderr, "Failed to write the entire string to the pipe.\n");
        return 1;
    }

    // Read the string from the read end of the pipe.
    char buffer[1024];
    ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer));
    if (bytes_read == -1) {
        fprintf(stderr, "Read from pipe error.\n");
        return 1;
    }
    if (bytes_read == 0) {
        fprintf(stderr, "Failed to read any data from the pipe.\n");
        return 1;
    } 

    // Print the string that was read from the pipe.
    // Add a null terminator, this indicates end of string.
    buffer[bytes_read] = '\0';
    printf("Read from the pipe: %s\n", buffer);

    return 0;
}
