#include "../include/parser.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>


command parse_command(char* input){
    int tokenIndex = 0; 
    int argsIndex = 0; 
    int optionsIndex = 0; 

    command command;

    //making sure the first arg and first opt are empty strings if none is present
    command.args[0] = ""; 
    command.options[0] = ""; 

    command.optionsNum = 0; 
    command.argsNum = 0; 

    char* strToken = strtok(input, " \t\n"); //spliting the input using the spaces and tab characters

    while( strToken != NULL ){
        if(tokenIndex == 0){
            command.name = strToken; //If it's the first token, then it's the command's name
        }
        else{
            //if the token is an option
            if(strToken[0] == '-'){
                command.options[optionsIndex] = strToken;
                optionsIndex++; 
                command.optionsNum++; 
            }//if the token is an option
            
            else{
                command.args[argsIndex] = strToken; 
                argsIndex++; 
                command.argsNum ++; 
            }
        }

        tokenIndex++; 
        strToken = strtok(NULL, " \t\n"); 
    }

    return command; 
}

char* read_input(){ 

    static int nonCanonMode = 0; // to check if the terminal is in non-canonical mode or not
    //if the terminal is in canonical mode, then we change it to non-canonical mode
    if(nonCanonMode == 0){ 
        struct termios oldt; 
        tcgetattr(STDIN_FILENO, &oldt); 
        oldt.c_lflag &= ~(ICANON | ECHO); 
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        nonCanonMode = 1; 
    }

    char* input = malloc(INPUT_MAX_SIZE * sizeof(char)); 
    char c; 
    int currentInputLength = 0; 
    int historyCommandCursor = nbHistoryCommands; //initializing the history cursor
    
    //getting the input char by characters
    while(1){
        c = getchar();
        //if the user pressed an escape key
        if(c == 27){
            getchar(); //ignoring the 2nd character
            c = getchar(); //getting the 3rd character

            //if it's the up arrow or the down arrow
            if(c == 'A' || c == 'B'){

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
                
            }//if it's the up arrow or the down arrow

        }//if the user pressed an escape key

        //deleting the last character if the delete key is pressed
        else if (c == 127){ 
            if(currentInputLength > 0){
                write(STDOUT_FILENO, "\b \b", sizeof("\b \b")); 
                input[currentInputLength-1] = '\0';
                currentInputLength -= 1; 
            }
        }//deleting the last character if the delete key is pressed

        else{
            write(STDOUT_FILENO, &c, sizeof(c));

            //if the enter key is pressed
            if(c == '\n'){
                add_to_history(input);
                break; 
            }
            else{
                input[currentInputLength] = c; 
                input[currentInputLength + 1] = '\0';  
                currentInputLength += 1;
            }
        }
    }//getting the input from the user char by characters
    
    return input; 
}


void add_to_history(char* command){

    FILE* historyFile = open_history_file('a'); 
    fprintf(historyFile, "%s\n", command); //adding the command to the history file

    nbHistoryCommands++; 

    //adding a slot to the history array
    historyCommands = realloc(historyCommands, nbHistoryCommands*sizeof(char*)); 
    //saving the command in the array containing the history commands
    historyCommands[nbHistoryCommands-1] = strdup(command);

    fclose(historyFile); 
}


void load_commands_history(){

    FILE* historyFile = open_history_file('r'); 
    char line[INPUT_MAX_SIZE]; 

    while( (fgets(line, sizeof(line), historyFile)) != NULL) {
        nbHistoryCommands++;
        line[strcspn(line, "\n")] = '\0'; //replacing the \n by \0

        //adding a slot to the history array
        historyCommands = realloc(historyCommands, nbHistoryCommands*sizeof(char*)); 
        //saving the command in the array containing the history commands
        historyCommands[nbHistoryCommands-1] = strdup(line); 
    }

    fclose(historyFile); 
}


FILE* open_history_file(char option){

    FILE* historyFile; 
    char* homePath = getenv("HOME");

    if(homePath != NULL){

        char filePath[strlen(homePath)]; 
        strcpy(filePath, homePath);
        strcat(filePath, "/.mini-bash_history"); //completing the path to the history file 

        //if we want to open the file to append to it
        if(option == 'a') {
            historyFile = fopen(filePath, "a");
        }

        //If we want to open the file to read from it
        else if (option == 'r') {
            historyFile = fopen(filePath, "r"); 
            //if the file doesn't existe
            if(historyFile == NULL){
                historyFile = fopen(filePath, "a"); //creating it
                fclose(historyFile); 
                historyFile = fopen(filePath, "r"); 
            }//if the file doesn't existe
        }
    }

    return historyFile; 
}