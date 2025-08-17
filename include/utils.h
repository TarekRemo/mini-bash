
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