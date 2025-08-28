#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "../include/parser.h"
#define REFRESH_PROMPT 401

extern int sigNum; 

/**
 * @brief Executes a command.
 * This function forks a new process and executes the command in the child process.
 * If the command is a builtin command, it calls 'execute_builtin' to execute it.
 * Otherwise, it calls execute_external to run the command as an external program.
 * If the child process is terminated or stopped by a signal, 
 *  the signal number is stored in the global variable 'sigNum'.
 * @param command The command structure containing the command name, options, and arguments.
 * @return Returns the exit status of the command if it was executed successfully,
 *  '-1' if the execution fails with an error, '-2' if the fork fails, 
 * '-3' if the child process is terminated by a signal,
 *  or '-4' if the child process was stopped.
 */
int execute(command command); 


/**
 * Executes a builtin command.
 * @param command The command structure containing the command.
 * @returns 0 if the command was executed successfully, REFRESH_PROMPT if the prompt should be refreshed or
 * -1 if it failed.
 */
int execute_builtin(command command); 

/**
 * @brief Executes an external command.
 * This function uses 'execvp' to execute the command in the current process.
 * It constructs an array of arguments to pass to 'execvp'.
 * @param command The command structure containing the command.
 * @return Returns -1 if the execution fails with an error.
 */
int execute_external(command command); 

#endif