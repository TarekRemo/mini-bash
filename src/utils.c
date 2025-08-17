#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/utils.h"


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
        if( strstr(pwd, home) != NULL ){
            int homeLength = strlen(home);
            int pwdLength = strlen(pwd); 

            //replacing the "home" directory string of the user by "~" 
            char newPwd[pwdLength-homeLength+2]; 
            newPwd[0] = '~'; 
            strcpy(newPwd + 1, pwd + homeLength); //copying the rest of the string after the "home" directory
            //replacing the "home" directory string of the user by "~" 

            strcpy(pwd, newPwd); //updating the pwd variable with the new string
        
        }//checking if the current directory is in the "home" directory of the user
    }

    int usernameLength = strlen(username);
    int pwdLength = strlen(pwd);

    //constructing the prompt string
    char* prompt = malloc(usernameLength + pwdLength + 12); // 12 for "mini-bash@", ":", and "$ "
    strcpy(prompt, "mini-bash@");
    strcat(prompt, username);
    strcat(prompt, ":"); 
    strcat(prompt, pwd);
    strcat(prompt, "$"); 
    //constructing the prompt string

    return prompt; 
}