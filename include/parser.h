#define MAX_OPTIONS 15
#define MAX_ARGS 15
#define INPUT_MAX_SIZE 256

/**
 * @file parser.h
 * @brief Header file for command parsing functionality.
 * This header file defines the structures and functions used for parsing commands
 * in a command-line interface. It includes the definition of the `command` structure,
 * which holds the command name, options, and arguments, as well as the functions
 * `parse_command` and `read_input` for processing user input.
 */

 /**
  * Structure representing a command.
  * It contains the command name, an array of options, an array of arguments,
  * and the counts of options and arguments.
  */
typedef struct command{
    char* name; 
    char* options[MAX_OPTIONS]; 
    char* args[MAX_ARGS]; 
    int optionsNum; 
    int argsNum; 
} command; 

/**
 * Parses a command from the input string.
 * @param input The input string containing the command.
 * @return A `command` structure containing the parsed command name, options, and arguments.
 * The first token is considered the command name, subsequent tokens are classified as options or arguments.
 * Options are identified by a leading hyphen ('-'), while arguments do not have this prefix.
 * The function uses `strtok` to split the input string based on spaces, tabs, and newlines.
 * The maximum number of options and arguments is defined by `MAX_OPTIONS` and `MAX_ARGS`, respectively.
 */
command parse_command(char* input); 

/**
 * Reads input from the standard input (stdin).
 * Allocates memory for the input string based on `INPUT_MAX_SIZE`.
 * @return A pointer to the input string read from stdin.
 * The input is read using `fgets`, which captures the entire line of input.
 * The caller is responsible for freeing the allocated memory after use.
 * If the input exceeds `INPUT_MAX_SIZE`, it will be truncated.
 */
char* read_input(); 