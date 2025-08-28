#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#define MAX_ARGS 30
#define INPUT_MAX_SIZE 256

#define NEXT_IS_OUTPUT_FILE 101
#define NEXT_IS_INPUT_FILE 102
#define IS_OUTPUT_FILE 103
#define IS_INPUT_FILE 104

//error values
#define UNEXPECTED_CHARACTER 105

/**
 * @file parser.h
 * @brief Header file for command parsing functionality.
 * This header file defines the structures and functions used for parsing commands
 * in a command-line interface.
 */

 /**
 * Array to hold the history of commands.
 * It is dynamically allocated and resized as needed.
 * The number of commands in history is tracked by `nbHistoryCommands`.
 * The history is loaded at the start of the program from the history
 * file located at the home directory of the user (e.g., `~/.mini-bash_history`).
 * The commands are stored as strings, and each command can be retrieved later.
 */
extern char** historyCommands; 


/**
 * Number of commands in the history.
 * It is initialized to zero and updated whenever a command is added or loaded from the history
 * file.
 */
extern int nbHistoryCommands; 


/**
* Structure representing a command.
* It contains the command name ('char* name'), an array of arguments ('char* args[]'),
* the counts of arguments ('int argsNum'), and fields for input/output redirection
* ('char* outputRedirFile', 'char* inputRedirFile').
*/
typedef struct command{
    char* name; 
    char* args[MAX_ARGS]; 
    int argsNum; 

    //to know if the redirection has to be in append mode (using ">>")
    int appendRedirect; 
    char* outputRedirFile;

    //to know if we have to redirect the input of the command to a specific file (using "<")
    char* inputRedirFile;
} command; 

/**
 * Parses a command from the input string.
 * @param input The input string containing the command.
 * @return A `command` structure containing the parsed command name, arguments, and redirection files.
 * @see command
 * In order to specifiy output redirection, use the '>' symbol followed by the filename.
 * To append to a file, use '>>' followed by the filename.
 * To specify input redirection, use the '<' symbol followed by the filename.
 * The function handles quoted arguments, allowing spaces within arguments enclosed in double quotes.
 * If an unexpected character is encountered (e.g., multiple redirection operators without a file),
 * the function sets `errno` to `UNEXPECTED_CHARACTER` and returns an empty command structure
 * The first token is considered the command name, subsequent tokens are classified as arguments.
 * The maximum number of arguments is defined by `MAX_ARGS`.
 */
command parse_command(char* input); 

/**
 * Adds a command to both the history file and the in-memory history array.
 * @param command The command string to be added to the history.
 */
void add_to_history(char* input); 

/**
 * Loads the command history from the history file into the in-memory history array.
 * The history file is read line by line, and each command is stored in the `historyCommands` array.
 * The number of commands loaded is tracked by `nbHistoryCommands`.
 * The history file is expected to be located at `~/.mini-bash_history`.
 * Each command is stored as a dynamically allocated string in the `historyCommands` array.
 * The function reallocates the `historyCommands` array as needed to accommodate the loaded commands.
 * The newline character at the end of each line is replaced with a null terminator to properly
 * terminate the strings.
 */
void load_commands_history(); 

/**
 * Opens the history file for reading or appending.
 * The history file is expected to be located at `~/.mini-bash_history`.
 * If the file does not exist, it will be automtically created.
 * @param option 'r' for reading, 'a' for appending.
 * @return A file pointer to the opened history file.
 */
FILE* open_history_file(char option); 

#endif