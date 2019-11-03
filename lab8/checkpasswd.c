#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 256

#define SUCCESS "Password verified\n"
#define INVALID "Invalid password\n"
#define NO_USER "No such user\n"

int main(void) {
    char user_id[MAXLINE];
    char password[MAXLINE];

    /* The user will type in a user name on one line followed by a password
    on the next.
    DO NOT add any prompts.  The only output of this program will be one
    of the messages defined above.
    */
    if(fgets(user_id, MAXLINE, stdin) == NULL) {
        perror("fgets");
        exit(1);
    }
    if(fgets(password, MAXLINE, stdin) == NULL) {
        perror("fgets");
        exit(1);
    }
    // TODO
    int fd[2];
    pipe(fd);

    pid_t pid = fork();
    if (pid == 0){
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        execlp("./validate", "validate", NULL);
    
    } else if (pid > 0){
        int status;
        close(fd[0]);
        write(fd[1], user_id, 10);
        write(fd[1], password, 10);
        wait(&status);
        
        int exitstatus = WEXITSTATUS(status);
        close(fd[1]);
        
        switch(exitstatus) {
            case 0:
                printf(SUCCESS);
            case 1:
                printf("there is an error");
            case 2:
                printf(INVALID);
            case 3:
                printf(NO_USER);
        }
    } else {
        perror("fork");
        exit(1);
    }
    return 0;
}
