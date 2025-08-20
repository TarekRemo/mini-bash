#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Gets the command prompt string.
 * This function constructs a prompt string that includes the username and the current working directory.
 * If the username or current directory is not set, it defaults to "user" and "/", respectively.
 * If the current directory is within the user's home directory, it replaces the home directory path
 * with a tilde ('~') for brevity.
 * The prompt string is formatted as "mini-bash@username:pwd$".
 * The caller is responsible for freeing the allocated memory for the prompt string.
 */
char* get_prompt(); 

/**
 * @brief Sets the terminal to canonical mode or non-canonical mode.
 * @param fd The file descriptor of the terminal.
 * @param status If 1, sets the terminal to canonical mode; if 0, sets it to non-canonical mode.
 */
void set_canonical_mode(int fd, int status); 

#endif