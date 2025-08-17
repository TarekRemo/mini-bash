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
  * Global variables for history file pointers.
  * These pointers are used to read and write command history to files.
  * `a_historyFile` is for appending history, while `r_historyFile`
  * is for reading history.
  * They are initialized to `NULL` and should be opened before use.
  * The files are expected to be opened in the appropriate modes
  * (e.g., append mode for `a_historyFile` and read mode for `r_historyFile`).
  */
  static FILE *a_historyFile = NULL;  
  static FILE *r_historyFile = NULL;

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

/**
 * appends the input command to the history file.
 * If the history file is not open, it will attempt to open it using `open_history_file`.
 * @param input The command input to be added to the history. 
 * @see open_history_file.
 */
void add_to_history(char* input); 

/**
 * Opens the history file for reading or appending.
 * The function initializes the global file pointers `a_historyFile` or `r_historyFile`
 * based on the provided option. If the file is already open, it will not reopen it
 * to avoid resource leaks.
 * The history file is expected to be located in the home directory of the user.
 * If the history file is missing, it will be created.
 * @param option A character indicating the mode to open the file:
 *               'a' for appending (write mode), 'r' for reading.
 */
void open_history_file(char option); 