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
                                                      "clear", "export", "unset"};

int is_builtin(char* commandName){
    for(int i = 0 ; i < NB_BUILTIN_COMMANDS ; i++){
        if(strcmp(commandName, builtinCommands[i]) == 0)
            return 1;
    }
    return 0;
}

int pwd(){
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
        return -1; 
    }
    else{
        printf("%s\n", currentDirectory); 
    }
    free(currentDirectory);
    return 0; 
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

int unset(command command){
    int counter = 0; //counter of the number of env var that has been successfully unset
    for(int i = 0 ; i < command.argsNum ; i++){
        if(unsetenv(command.args[i]) == -1)
            perror("Une erreur s'est produite"); 
        else
            counter++;
    }
    return counter; 
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