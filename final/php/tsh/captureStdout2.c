#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void main () {
	int filedes[2];
	if (pipe(filedes) == -1) {
	  perror("pipe");
	  exit(1);
	}
	pid_t pid = fork();

	if (pid == -1) {
	  perror("fork");
	  exit(1);
	} else if (pid == 0) {
	  while ((dup2(filedes[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {};
	  close(filedes[1]); // 
	  close(filedes[0]);
	  char *argv[] = {"/bin/ls", "-la", 0};
	  execv(argv[0], argv);
	  perror("execv error");
	  _exit(1);
	}
	// Parent
	close(filedes[1]); // This is the side the child will write
	char buffer[4096];
	while (1) {
		ssize_t count = read(filedes[0], buffer, sizeof(buffer));
		if (count == -1) {
			if (errno == EINTR) {
				perror("Interrupted?");
				continue;
			} else {
				perror("Pipe read error");
				exit(1);
			}
		} else if (count == 0) {
			printf("Nothing to read. count=0 ??? \n");
			break;
		} else {
			printf("Captured[%s](%ld)\n",buffer, count);
		}	
	}
	close(filedes[0]);
	wait(0);
	printf("All done! \n");
}