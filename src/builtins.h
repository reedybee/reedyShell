#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int rshell_builtin_cd(char** args);
int rshell_builtin_help(char** args);
int rshell_builtin_exit(char** args);

char* builtin_cmds[] = {
    "cd",
    "help",
    "exit"
};

char* builtin_desc[] = {
    "cd [directory]  changes current directory",
    "help            prints help page",
    "exit            exits the current process"
};

int (*builtin_func[])(char** ) = {
    &rshell_builtin_cd,
    &rshell_builtin_help,
    &rshell_builtin_exit
};

int rshell_num_builtins(void) {
    return sizeof(builtin_cmds) / sizeof(char*);
}

int rshell_builtin_cd(char** args) {
    if (args[1] == NULL) {
        fprintf(stderr, "bicd1 reedyShell: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 1) {
            perror("bicd2 reedyShell");
        }
    }
    return 1;
}

int rshell_builtin_help(char** args) {
    printf("Reedybee's Shell\n\n");
    printf("The following commands are built in:\n");

    for (int i = 0; i < rshell_num_builtins(); i++) {
        printf("  %s:  %s\n", builtin_cmds[i], builtin_desc[i]);
    }

    return 1;
}

int rshell_builtin_exit(char** args) {
    return 0;
}