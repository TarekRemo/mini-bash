#define MAX_OPTIONS 15
#define MAX_ARGS 15
#define INPUT_MAX_SIZE 256

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
static char** historyCommands; 

/**
 * Number of commands in the history.
 * It is initialized to zero and updated whenever a command is added or loaded from the history
 * file.
 */
static int nbHistoryCommands; 

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
 * Reads input from the user in non-canonical mode.
 * The function reads characters one by one, allowing for immediate processing of input.
 * special keys handeled are the up and down arrows to navigate through command history 
 * and the enter key to submit the command.
 * @return the input string entered by the user once the enter key is pressed.
 * The input is allocated dynamically and should be freed by the caller using `free()`.
 */
char* read_input(); 

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