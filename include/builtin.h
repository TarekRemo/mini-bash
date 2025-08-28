#ifndef BUILTIN_H
#define BUILTIN_H 

#include "../include/parser.h"

#define NB_BUILTIN_COMMANDS 9


/**
 * Checks if a command is a builtin command.
 * @returns 1 if the command is a builtin command, 0 otherwise.
 */
int is_builtin(char* command); 

/**
 * shows the current working directory.
 * It prints the current directory to the standard output.
 * If the current directory cannot be determined, it prints an error message.
 * @returns 0 if the operation was successful, -1 otherwise.
 */
int pwd(); 

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
 * unsets environment variables.
 * Does nothing if no arguments are present int the command parameter.
 * The arguments should not contain the '=' character.
 * @param command The command containing the arguments to unset.
 * @return the number of environment variables that were successfully unset.
 */
int unset(command command); 

/**
 * Exports environment variables.
 * If no arguments are provided, it prints the list of environment variables.
 * If arguments are provided, it sets the environment variables to the specified values.
 * The arguments should be in the format "VAR=VALUE".
 * If the format is incorrect, it prints an error message.
 * If the variable already exists, it updates its value.
 * @param command The command containing the arguments to export.
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