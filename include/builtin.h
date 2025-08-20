#ifndef BUILTIN_H
#define BUILTIN_H 

#include "../include/parser.h"

#define NB_BUILTIN_COMMANDS 8
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
 * Shows the command history.
 * It prints the list of commands stored in the history to the standard output.
 */
void history(); 

/**
 * Exports environment variables.
 * If no arguments are provided, it prints the list of environment variables.
 * If arguments are provided, it sets the environment variables to the specified values.
 * @param command The command containing the arguments to export.
 * The arguments should be in the format "VAR=VALUE".
 * If the format is incorrect, it prints an error message.
 * If the variable already exists, it updates its value.
 * @returns the number of environment variables that were successfully set.
 */
int export(command command);

/**
 * Changes the current working directory to the specified path in the command args.
 * If the path is not provided, it changes to the home directory.
 * If the path is relative, it appends it to the current directory.
 * If the path is absolute, it changes to that directory.
 * If the path does not exist or is not a directory, it prints an error message.
 * If the operation is successful, it updates the PWD and OLDPWD environment variables.
 * @param command The command containing the path to change to.
 * @returns 0 if the operation was successful, -1 otherwise.
 */
int cd(command command); 

#endif