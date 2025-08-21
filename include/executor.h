#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "../include/parser.h"

/**
 * @brief Executes a command using the execvp function.
 * This function forks a new process and executes the command in the child process.
 * It handles command options and arguments, and returns the result of the execution.
 * If the command execution fails, it prints an error message to stderr.
 * @param command The command structure containing the command name, options, and arguments.
 * @return Returns the exit status of the command if it was executed successfully,
 *  '-1' if the execvp fails with an error, '-2' if the fork fails, 
 * '-3' if the child process is terminated by a signal,
 *  or '-4' if the child process was stopped.
 */
int execute(command command); 

#endif