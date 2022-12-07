#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include<fcntl.h> 

#define BUFFER_SIZE 1024

int main(void) {
    int pipefd[2];
    if (pipe(pipefd) < 0) {
		fprintf(stderr, "Create pipe error\n");
        return 1;
    }

	// set for non block write
    fcntl(pipefd[1], F_SETFL, O_NONBLOCK); 


    // Write a large amount of data to the pipe.
    char buffer[BUFFER_SIZE];
    memset(buffer, 'X', sizeof(buffer));

    size_t bytes_written = 0;
    while (1) {
        // Write a chunk of data to the pipe.
        ssize_t ret = write(pipefd[1], buffer, sizeof(buffer));

        // If an error occurred or no data 
		// was written, exit the loop.
        if (ret == -1 || ret == 0) break;

        // Update the total number of bytes written.
        bytes_written += ret;
    }
    printf("Wrote %zu bytes to the pipe.\n", bytes_written);
    return 0;
}
