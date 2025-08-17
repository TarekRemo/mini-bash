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
            }
            
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
    return input; 
}