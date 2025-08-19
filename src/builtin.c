#include "../include/builtin.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include<linux/limits.h>

/**
 * Array of builtin commands.
 */
static char* builtinCommands[NB_BUILTIN_COMMANDS] = {"cd", "pwd"};

int execute_builtin(command command){

    if(strcmp(command.name, "cd") == 0){
        if (cd(command.args[0]) == 0)
            return REFRESH_PROMPT; // If cd is successful, tell the caller to refresh the prompt
    }

    else if (strcmp(command.name, "pwd") == 0){
        pwd(); 
        return 0; 
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


int cd(char* path){

    //if the user wants to go to the home directory
    if(strcmp(path, "") == 0 || path == NULL || !path ){
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