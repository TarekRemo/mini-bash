#ifndef BUILTIN_H
#define BUILTIN_H 

#include "../include/parser.h"

#define NB_BUILTIN_COMMANDS 5
#define REFRESH_PROMPT 401

/**
 * Executes a builtin command.
 * @returns REFRESH_PROMPT if the command was executed successfully and the prompt should be refreshed or
 * -1 if it failed or if it's not a builtin command.
 */
int execute_builtin(command command); 


/**
 * Checks if a command is a builtin command.
 * @returns 1 if the command is a builtin command, 0 otherwise.
 */
int is_builtin(char* command); 

/**
 * shows the current working directory.
 * It prints the current directory to the standard output.
 * If the current directory cannot be determined, it prints an error message.
 */
void pwd(); 

/**
 * Shows the help message for builtin commands.
 * It prints the list of available builtin commands to the standard output.
 */
void help(); 

/**
 * Changes the current working directory to the specified path.
 * @returns 0 if the operation was successful, -1 otherwise.
 * If the path is ".", it changes to the home directory.
 * If the path is relative, it appends it to the current directory.
 * If the path is absolute, it changes to that directory.
 * If the path does not exist or is not a directory, it prints an error message.
 * If the operation is successful, it updates the PWD and OLDPWD environment variables.
 */
int cd(char* path); 

#endif