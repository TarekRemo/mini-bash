#include "../include/builtin.h"
#include "../include/utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <linux/limits.h>

/**
 * Array of builtin commands.
 */
static char* builtinCommands[NB_BUILTIN_COMMANDS] = {"cd", "pwd", "exit",
                                                     "echo", "help", "history",
                                                      "clear", "export"};

int execute_builtin(command command){

    if(strcmp(command.name, "cd") == 0){
        if (cd(command) == 0)
            return REFRESH_PROMPT; // If cd is successful, tell the caller to refresh the prompt
    }

    else if (strcmp(command.name, "pwd") == 0){
        pwd(); 
        return 0; 
    }

    else if(strcmp(command.name, "exit") == 0){
        set_canonical_mode(STDIN_FILENO, 1); //reactivating the canonical mode for the terminal
        exit(EXIT_SUCCESS); 
        return 0; 
    }

    else if(strcmp(command.name, "echo") == 0){
        printf("%s\n", command.args[0]); 
    }

    else if(strcmp(command.name, "help") == 0){
        help();  
    }

    else if(strcmp(command.name, "history") == 0){
        history();  
    }

    else if(strcmp(command.name, "clear") == 0){
        printf("\033[H\033[J"); 
    }

    else if(strcmp(command.name, "export") == 0){
        export(command);  
    }

    return -1;
}


int is_builtin(char* commandName){
    for(int i = 0 ; i < NB_BUILTIN_COMMANDS ; i++){
        if(strcmp(commandName, builtinCommands[i]) == 0)
            return 1;
    }
    return 0;
}

void pwd(){
    char* currentDirectory = malloc(PATH_MAX); 
    currentDirectory = getcwd(currentDirectory, PATH_MAX); 

    if(currentDirectory == NULL){
        switch(errno){
            case ERANGE :
                perror("la taille du chemin est trop grand"); 
                break; 
            case EACCES :
                perror("Vous n'avez pas les permission nécessaires"); 
                break; 
            case ENOMEM: 
                perror("Espace insuffisant"); 
                break; 
            default:
                perror("Une erreur est survenue"); 
                break;  
        }
    }
    else{
        printf("%s\n", currentDirectory); 
    }

    free(currentDirectory);
}

void help(){
    printf("Builtin commands : \n"); 
    for(int i = 0 ; i < NB_BUILTIN_COMMANDS ; i++){
        printf("%s\n", builtinCommands[i]); 
    } 
}

void history(){
    for(int i = 0 ; i < nbHistoryCommands ; i++){
        printf("%s\n", historyCommands[i]); 
    }
}

int export(command command){

    int counter = 0; //counter of the number of env var that has been successfully set

    //if there is no args, then printing the list of env variables
    if(command.argsNum == 0){ 
        for(int i = 0 ; __environ[i] != NULL ; i++)
            printf("%s\n", __environ[i]); 
    }

    else{
        char* arg; 
        char* varName; 
        char* varValue;
        int nameSize; 

        for(int i = 0 ; i < command.argsNum ; i++){

            arg = strdup(command.args[i]); 
            varValue = strchr(arg, '='); 
            if(varValue == NULL){
                printf("impossible d'exporter '%s'. le format doit être la suivante : nomVar=ValeurVar\n", arg);
                continue; 
            }

            nameSize = strlen(arg)-strlen(varValue); 
            varName = malloc(nameSize); 
 
            varName = strncpy(varName, arg, nameSize);
            varName[nameSize] = '\0'; //ending the name's string
            varValue = varValue+1; //removing the '=' from the value

            setenv(varName, varValue, 1); 
            free(varName); 
            counter++; 
        }
    }

    return counter; 
}

int cd(command command){

    char* path = command.args[0];

    //if the user wants to go to the home directory
    if(command.argsNum == 0){
        path = getenv("HOME"); 
    }

    int result = chdir(path); 

    if(result == -1){
        switch(errno){
            case EACCES: 
                perror("Vous n'avez pas les droits nécessaires pour accéder à ce dossier"); 
                break; 

            case ENOENT: 
                perror("le chemin spécifié n'existe pas"); 
                break; 
            
            case ENOTDIR: 
                perror("un des éléments du chemin spécifié n'est pas un dossier"); 
                break; 
            
            default: 
                perror("une erreur s'est produite"); 
                break; 
        }
    }
    else{ //changing the PWD and OLDPWD environment variables

        char* pwd = getenv("PWD"); 
        if(pwd == NULL)
            strcpy(pwd, "/"); 

        char* oldPwd = strdup(pwd);

        pwd = malloc(PATH_MAX); 
        pwd = getcwd(pwd, PATH_MAX); 

        setenv("PWD", pwd, 1); //updating the PWD env var
        setenv("OLDPWD", oldPwd, 1); //updating the OLDPWD env var
        free(pwd); 

    } //changing the PWD and OLDPWD environment variables
    
    return result; 
}