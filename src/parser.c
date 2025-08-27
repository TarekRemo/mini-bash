#include "../include/parser.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

char** historyCommands = NULL;
int nbHistoryCommands;

command parse_command(char* input){
    int tokenIndex = 0; 
    int argsIndex = 0; 

    command command;

    //making sure the first arg and first opt are empty strings if none is present
    command.args[0] = ""; 
    command.argsNum = 0; 

    char* ptr = input;
    while (*ptr) {
        // Skip leading spaces/tabs
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') 
            ptr++;

        if (*ptr == '\0') 
            break;

        char token[INPUT_MAX_SIZE];
        int tokenLen = 0;

        //If the argument is between quotes
        if (*ptr == '"') {

            ptr++; // skip opening quote
            while (*ptr && *ptr != '"') {
                token[tokenLen] = *ptr;
                tokenLen++;
                ptr++;
            }
            token[tokenLen] = '\0';

            if (*ptr == '"') 
                ptr++; // skip closing quote

        } //If the argument is between quotes

        // normal argument
        else {
            while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') {
                token[tokenLen] = *ptr;
                tokenLen++;
                ptr++;
            }
            token[tokenLen] = '\0';
        }// normal argument

        if (tokenLen > 0) {
            if (tokenIndex == 0) {
                command.name = strdup(token);
            }
            else {
                command.args[argsIndex] = strdup(token);
                argsIndex++;
                command.argsNum++;
            }
            tokenIndex++;
        }
    }

    return command; 
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
    
    historyCommands = malloc(0);
    nbHistoryCommands = 0;

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