#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
    // Create two pipes
    int pipe1[2], pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
		perror("Create pipe error!");
		exit(EXIT_FAILURE);
    }
  
    // Create a child process
    pid_t pid = fork();
    if (pid == -1) {
		perror("Fork new process error!");
		exit(EXIT_FAILURE);
    }
  
    if (pid == 0) { // child process
		char buf[BUFSIZE];
	  
		// Close the unused end of the pipes
		close(pipe1[1]);
		close(pipe2[0]);
	  
		// Read from pipe1 and print
		while (read(pipe1[0], buf, BUFSIZE) > 0) {
		    printf("Consumer: received '%s' from producer\n", buf);

		    // Write back to pipe2
		    if (write(pipe2[1], "ACK", 3) == -1) {
				perror("Write back to pipe2 error!");
			    exit(EXIT_FAILURE);
		    }
		}
	  
		// Close the used ends of the pipes
		close(pipe1[0]);
		close(pipe2[1]);
    } else { // parent process
		char buf[BUFSIZE];
	  
		// Close the unused end of the pipes
		close(pipe1[0]);
		close(pipe2[1]);
	  
		// Read input from user and write to pipe1
		while (fgets(buf, BUFSIZE, stdin) != NULL) {
		    if (write(pipe1[1], buf, strlen(buf)) == -1) {
		  		perror("Write to pipe1 error");
				exit(EXIT_FAILURE);
			}
	  
			// Read from pipe2 and print
		 	if (read(pipe2[0], buf, BUFSIZE) > 0) {
			    printf("Producer: received '%s' from consumer\n", buf);
			}
		}
	  
		// Close the used ends of the pipes
		close(pipe1[1]);
		close(pipe2[0]);
    }
  
    return 0;
}
