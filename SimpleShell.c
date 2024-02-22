#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_ARGS 128
#define MAX_INPUTTS 1000

// Tested and worked: pwd, mkdir, rmdir, rm -rf, ls, echo, exit, &, disabled ^C
// Tested and not worked with only execvp: cd, pipe, redirection (echo "hello" > hello.txt)

void ignore_control_C() {
    signal(SIGINT, SIG_IGN); // Ignore ^C signal
}

int main(){
    // Your shell should parse the command line to build the argv array
    char *argv[MAX_ARGS];
    char input[MAX_INPUTTS];
    int argc = 0;
    int pid;
    int background = 0; // Flag for background execution, set to 1 if '&' detected at command end.

    argv[argc] = NULL; // make sure the last element in argv is '/0' or NULL

    ignore_control_C(); //comment out for debugging purpose

    printf("Hello World to terminal. To quit, enter 'exit'.\n\n");

    while(1){
        printf("SimpleShell-> ");
        //fflush(stdout);

        // Read input
        if (fgets(input, sizeof(input), stdin) == NULL) { // Error or EOF
            continue;
        }

        // Check for newline character and replace it with null terminator
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) { // if input equal to 'exit', exit SimpleShell
            break;
        }

        argc = 0;
        background = 0;
        
        char *token = strtok(input, " ");
        while (token != NULL) {
            if (strcmp(token, "&") == 0) { // The shell must also interpret the “&” operator as a command terminator. 
                background = 1; // Set background execution flag
                break; 
            }
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }
        argv[argc] = NULL; // Null-terminate argv at the last char

        if (argc == 0) { // if input empty command, stop and return to beginning of loop
            continue;
        }

        pid = fork();
        if (pid == 0) { // Child process if pid = 0
            if (execvp(argv[0], argv) == -1) {
                perror("execvp failed"); // If a command is given that execvp() cannot execute (such as an erroneous command), an appropriate error message must be displayed.
                exit(EXIT_FAILURE);
            }
        } else if (pid > 0) { // Parent process
            if (!background) { // Wait for the child process to complete if not running in the background.
                wait(NULL);
            }
        } else { // Fork failed
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
    
    }
}
