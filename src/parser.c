#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h" 

command parse_command(char* input){
 
    int  tokenIndex = 0; 
    int argsIndex = 0; 
    int optionsIndex = 0; 

    command command;
    command.optionsNum = 0; 
    command.argsNum = 0; 

    char* strToken = strtok(input, " \t\n"); //spliting the input using the spaces and tab characters

    while( strToken != NULL ){

        if(tokenIndex == 0){
            command.name = strToken; //If it's the first token, then it's the command's name
        }

        else{

            //if the token is an option
            if(strstr(strToken, "-")){
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
    long int size = INPUT_MAX_SIZE * sizeof(char); 
    char* input = malloc(size); 

    fgets(input, size, stdin);
    add_to_history(input); 

    return input; 
}


void add_to_history(char* input){
    if(!a_historyFile)
        open_history_file('a'); 
    
    fprintf(a_historyFile, "%s", input); 
    fflush(a_historyFile); 
}


void open_history_file(char option){
    char* path = getenv("HOME");

    if(path != NULL){
        strcat(path, "/.mini-bash_history"); //completing the path to the history file 

        if(option == 'a' && a_historyFile == NULL) //if we want to open the file to append to it
                a_historyFile = fopen(path, "a");

        //If we want to open the file to read from it
        else if (option == 'r' && r_historyFile == NULL) {
            r_historyFile = fopen(path, "r"); 

            //if the file doesn't existe
            if(r_historyFile == NULL){
                r_historyFile = fopen(path, "a"); //creating it
                fclose(r_historyFile); 
                r_historyFile = fopen(path, "r"); 
            }//if the file doesn't existe

        }//If we want to open the file to read from it
    }
}