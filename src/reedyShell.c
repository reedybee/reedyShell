#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "builtins.h"

#define R_SHELL_BUFSIZE 1024
#define R_SHELL_TOK_BUFSIZE 64
#define R_SHELL_TOK_DELIM " \t\r\n\a"

char* rshell_read_line(void);
char** rshell_split_line(char *line);
void rshell_loop(void);
int rshell_launch(char** args);
int rshell_execute(char** args);

// reedyShell read line
char* rshell_read_line(void) {
  char *line = NULL;
  ssize_t bufsize = 0; // have getline allocate a buffer for us

  if (getline(&line, &bufsize, stdin) == -1){
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);  // We recieved an EOF
    } else  {
      perror("readline");
      exit(EXIT_FAILURE);
    }
  }

  return line;
}

char** rshell_split_line(char* line) {
  int bufsize = R_SHELL_TOK_BUFSIZE, position = 0;
  char** tokens = malloc(bufsize * sizeof(char*));
  char* token;

  if (!tokens) {
    fprintf(stderr, "ReedyShell Allocation Error\n");
    exit(1);
  }

  token = strtok(line, R_SHELL_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += R_SHELL_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "ReedyShell Allocation Error\n");
        exit(1);
      }
    }

    token = strtok(NULL, R_SHELL_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

// reedyShell loop
void rshell_loop(void) {
  char* line;
  char** args;
  int status;

  do {
      printf("> ");

      line = rshell_read_line();
      args = rshell_split_line(line);
      status = rshell_execute(args);

      free(line);
      free(args);

  } while (status);
}

int rshell_launch(char** args) {
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("launch1 reedyShell");
    }
    exit(1);
  } else if (pid < 0) {
    perror("launch2 reedyShell");
  } else {
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int rshell_execute(char** args) {

  if (args[0] == NULL) {
    return 1;
  }

  for (int i = 0; i < rshell_num_builtins(); i++) {
    if (strcmp(args[0], builtin_cmds[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return (rshell_launch(args));
}

// main
int main(int argc, char* argv[]) {

  // load config files eventually
  
  // loop
  rshell_loop();

  // cleanup
  return 0;
}