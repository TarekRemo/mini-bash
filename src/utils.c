#include "../include/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



char* get_prompt(){
    char* username = getenv("USERNAME"); 

    if(username == NULL){
        username = "user";
    }

    char* pwd = getenv("PWD"); 

    if(pwd == NULL){
        pwd = "/"; 
        setenv("PWD", pwd, 1); //setting the current directory to the root of the file system
    } 

    else{
        char* home = getenv("HOME"); 

        //checking if the current directory is in the "home" directory of the user
        if( strncmp(pwd, home, strlen(home)) == 0 ){
            int homeLength = strlen(home);
            int pwdLength = strlen(pwd); 
            char* newPwd = malloc(pwdLength - homeLength + 2); // +2 for '~' and '\0'

            //replacing the "home" directory string of the user by "~" 
            newPwd[0] = '~'; 
            strcpy(newPwd + 1, pwd + homeLength); //copying the rest of the string after the "home" directory
            //replacing the "home" directory string of the user by "~" 

            pwd = newPwd;        
        }//checking if the current directory is in the "home" directory of the user
    }

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

    if(pwd[0] == '~'){
        //if the pwd starts with "~", we need to free the memory allocated for the newPwd
        free(pwd);
    }

    return prompt; 
}