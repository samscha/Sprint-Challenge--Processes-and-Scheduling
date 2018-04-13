#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define PROMPT "lambda-shell$ "

#define MAX_TOKENS 100
#define COMMANDLINE_BUFSIZE 1024
#define DEBUG 0 // Set to 1 to turn on some debugging output, or 0 to turn off

/**
 * Parse the command line.
 *
 * YOU DON'T NEED TO MODIFY THIS!
 * (But you should study it to see how it works)
 *
 * Takes a string like "ls -la .." and breaks it down into an array of pointers
 * to strings like this:
 *
 *   args[0] ---> "ls"
 *   args[1] ---> "-la"
 *   args[2] ---> ".."
 *   args[3] ---> NULL (NULL is a pointer to address 0)
 *
 * @param str {char *} Pointer to the complete command line string.
 * @param args {char **} Pointer to an array of strings. This will hold the result.
 * @param args_count {int *} Pointer to an int that will hold the final args count.
 *
 * @returns A copy of args for convenience.
 */
char **parse_commandline(char *str, char **args, int *args_count)
{
  char *token;

  *args_count = 0;

  token = strtok(str, " \t\n\r");

  while (token != NULL && *args_count < MAX_TOKENS - 1)
  {
    args[(*args_count)++] = token;

    token = strtok(NULL, " \t\n\r");
  }

  args[*args_count] = NULL;

  return args;
}

/**
 * Main
 */
int main(void)
{
  // Holds the command line the user types in
  char commandline[COMMANDLINE_BUFSIZE];

  // Holds the parsed version of the command line
  char *args[MAX_TOKENS];

  // How many command line args the user typed
  int args_count;

  // Shell loops forever (until we tell it to exit)
  while (1)
  {
    // Print a prompt
    printf("%s", PROMPT);
    fflush(stdout); // Force the line above to print

    // Read input from keyboard
    fgets(commandline, sizeof commandline, stdin);

    // Exit the shell on End-Of-File (CRTL-D)
    if (feof(stdin))
    {
      break;
    }

    // Parse input into individual arguments
    parse_commandline(commandline, args, &args_count);

    if (args_count == 0)
    {
      // If the user entered no commands, do nothing
      continue;
    }

    // Exit the shell if args[0] is the built-in "exit" command
    if (strcmp(args[0], "exit") == 0)
    {
      break;
    }

#if DEBUG

    // Some debugging output

    // Print out the parsed command line in args[]
    for (int i = 0; args[i] != NULL; i++)
    {
      printf("%d: '%s'\n", i, args[i]);
    }

#endif

    /* Add your code for implementing the shell's logic here */
    if (strcmp(args[0], "cd") == 0)
    {
      if (args_count == 2)
      {

        if (chdir(args[1]) == -1)
          perror("chdir");
      }

      continue;
    }
    /* Why is cd built into the shell? Why can't it run as an external command? */
    /* cd is built into the shell because cd is just a pointer to an array 
       (in this case, persistent storage of laptop/computer). No shell command
       is run. It's just a change in file pointing.
     */

    int waiting = 1;

    if (strcmp(args[args_count - 1], "&") == 0)
    {
      args[args_count - 1] = NULL;
      waiting = 0;

      if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) /* http://www.microhowto.info/howto/reap_zombie_processes_using_a_sigchld_handler.html */
      {
        perror(0);
        exit(1);
      }
    }

    int rc = fork();

    if (rc < 0)
    {
      fprintf(stderr, "fork() failed\n");
      exit(1);
    }
    else if (rc == 0)
    {
      execvp(args[0], args);
    }

    if (waiting > 0)
      waitpid(rc, NULL, 0);

    /* What happens if you don't do this? */
    /* If we don't reap zombie processes, then they will never be offloaded
         from main mem. These processes will be "exit"ed but the process table
         will still show them. This happens because the parent process never
         reads the child's (now zombie's) exit code (via `wait()`).
      */
  }

  return 0;
}
