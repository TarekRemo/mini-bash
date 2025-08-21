#include "../include/utils.h"
#include "../include/parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termio.h>
#include  <linux/limits.h>



char* get_prompt(){

    char* username = getenv("USERNAME"); 
    if(username == NULL){
        username = "user";
    }

    char* pwd = malloc(PATH_MAX); 
    pwd = getcwd(pwd, PATH_MAX); 

    if(pwd == NULL){
        strcpy(pwd, "/");  
        chdir(pwd); //setting the current directory to the root of the file system
        setenv("PWD", pwd, 1); //updating the PWD variable
    } 

    char* home = getenv("HOME"); 

    //checking if the current directory is in the "home" directory of the user
    if( strncmp(pwd, home, strlen(home)) == 0 ){
        int homeLength = strlen(home);
        char* cpy = strdup(pwd); 
        pwd[0] = '~'; 
        strcpy(pwd+1, cpy+homeLength);      
    }//checking if the current directory is in the "home" directory of the user
    

    int usernameLength = strlen(username);
    int pwdLength = strlen(pwd);

    //constructing the prompt string
    char* prompt = malloc(usernameLength + pwdLength + 32 ); // 34 for "mini-bash@", ":", and "$ " and color codes
    strcpy(prompt, "\033[33mmini-bash@"); //yellow color for "mini-bash@" and username
    strcat(prompt, username);
    strcat(prompt, "\033[37m:\033[34m"); //white color for the colon and blue for the pwd
    strcat(prompt, pwd);
    strcat(prompt, "\033[0m$"); //default color reset and dollar sign
    //constructing the prompt string

    free(pwd);

    return prompt; 
}

void set_canonical_mode(int fd, int status){
    struct termios oldt; 
    tcgetattr(fd, &oldt); 
    if(status == 0)
        oldt.c_lflag &= ~(ICANON | ECHO); 
    else if (status == 1)
        oldt.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

char* read_input(){ 

    static int nonCanonMode = 0; // to check if the terminal is in non-canonical mode or not
    //if the terminal is in canonical mode, then we change it to non-canonical mode
    if(nonCanonMode == 0){ 
        set_canonical_mode(STDIN_FILENO, 0); 
        nonCanonMode++;
    }

    char* input = malloc(INPUT_MAX_SIZE * sizeof(char)); 
    char c; 
    int currentInputLength = 0; 
    int historyCommandCursor = nbHistoryCommands; //initializing the history cursor
    int inputCursor = 0;  

    //getting the input char by characters
    while(1){

        c = getchar();

        //if the user pressed an escape key
        if(c == 27){
            getchar(); //ignoring the 2nd character
            c = getchar(); //getting the 3rd character

            //if it's the up arrow or the down arrow
            if(c == 'A' || c == 'B'){

                //making sur the input cursor is at the end of the current input to delete it
                for(; inputCursor < currentInputLength ; inputCursor++)
                    write(STDOUT_FILENO, "\033[C", sizeof("\033[C"));

                //deleting the current input from the terminal
                for(int i = 0 ; i < currentInputLength ; i++)
                    write(STDOUT_FILENO, "\b \b", sizeof("\b \b")); 
                
                if(c == 'A' && historyCommandCursor > 0){
                    historyCommandCursor--; //moving the history cursor up
                    strcpy(input, historyCommands[historyCommandCursor]); //copying the command
                }

                else if(c == 'B' && historyCommandCursor < nbHistoryCommands){
                    historyCommandCursor++; 
                    if(historyCommandCursor == nbHistoryCommands)
                        strcpy(input, ""); 
                    else
                        strcpy(input, historyCommands[historyCommandCursor]); //getting the latest history command
                }

                write(STDOUT_FILENO, input, strlen(input)*sizeof(char)); //showing it on the terminal
                currentInputLength = strlen(input);
                inputCursor = currentInputLength; 
                
            }//if it's the up arrow or the down arrow

            //if it's the left arrow
            else if(c == 'D' && 0 < inputCursor){
                write(STDOUT_FILENO, "\b", sizeof("\b")); 
                inputCursor--;
            }

            //if it's the left arrow
            else if(c == 'C' && inputCursor < currentInputLength){
                write(STDOUT_FILENO, "\033[C", sizeof("\033[C")); 
                inputCursor++;
            }

        }//if the user pressed an escape key

        //deleting the last character if the delete key is pressed
        else if (c == 127){ 
            if(inputCursor > 0){
                // Moving every charaater one position to the left
                memmove(input+inputCursor-1, input+inputCursor, currentInputLength - inputCursor);
                currentInputLength -= 1;
                input[currentInputLength] = '\0';
                inputCursor--;

                // deleting the line starting from the cursor
                write(STDOUT_FILENO, "\b", 1);
                write(STDOUT_FILENO, input+inputCursor, strlen(input+inputCursor));
                write(STDOUT_FILENO, " ", 1); // deleting the last character
                // putting the cursor back to the right position
                for(int i = 0; i < (int) strlen(input+inputCursor)+1; i++)
                write(STDOUT_FILENO, "\b", 1);
            }
        }//deleting the last character if the delete key is pressed

        else{
            //if the enter key is pressed
            if(c == '\n'){
                write(STDOUT_FILENO, "\n", 1); //printing a new line
                add_to_history(input);
                break; 
            }
            else{
                // moving every character one position to the right
                memmove(input + inputCursor + 1, input + inputCursor, currentInputLength - inputCursor);
                input[inputCursor] = c;
                currentInputLength++;
                inputCursor++;
                input[currentInputLength] = '\0';

                // showing the line starting from the cursor
                write(STDOUT_FILENO, input + inputCursor - 1, strlen(input + inputCursor - 1));
                // putting the cursor back to the right position
                for(int i = 0; i < (int)strlen(input + inputCursor); i++){
                    write(STDOUT_FILENO, "\b", 1);
                }
            }
        }//getting the input from the user char by characters
    }
    return input; 
}