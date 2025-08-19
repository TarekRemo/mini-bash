#include "../include/builtin.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int execute_builtin(command command){

    if(is_builtin(command.name) == 0){
        return -1;
    }

    if(strcmp(command.name, "cd") == 0){
        if (cd(command.args[0]) == 0)
            return REFRESH_PROMPT; // If cd is successful, tell the caller to refresh the prompt
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


int cd(char* path){

    //if the user wants to go to the home directory
    if(strcmp(path, ".") == 0)
        strcpy(path, getenv("HOME"));

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
        char* oldPwd = strdup(pwd);

        if(path[0] == '.'){
            if(path[1] == '/')
                strcat(pwd, path+1); //replacing the dot with the current directory

            else if (path[1] == '.'){
                char* lastDir = strrchr(pwd, '/'); 
                lastDir[0] = '\0'; //removing the last directory from the path
            }
        }

        else if (path[0] != '/'){ //adding the current directory to the path if it is not absolute
            strcat(pwd, "/"); 
            strcat(pwd, path); 
        }

        else{
            strcpy(pwd, path); 
        }

        setenv("PWD", pwd, 1); //updating the PWD env var
        setenv("OLDPWD", oldPwd, 1); //updating the OLDPWD env var

    } //changing the PWD and OLDPWD environment variables
    
    return result; 
}