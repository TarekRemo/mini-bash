#include "../include/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termio.h>
#include  <linux/limits.h>



char* get_prompt(){

    char* username = getenv("USERNAME"); 
    if(username == NULL){
        username = "user";
    }

    char* pwd = malloc(PATH_MAX); 
    pwd = getcwd(pwd, PATH_MAX); 

    if(pwd == NULL){
        strcpy(pwd, "/");  
        chdir(pwd); //setting the current directory to the root of the file system
        setenv("PWD", pwd, 1); //updating the PWD variable
    } 

    char* home = getenv("HOME"); 

    //checking if the current directory is in the "home" directory of the user
    if( strncmp(pwd, home, strlen(home)) == 0 ){
        int homeLength = strlen(home);
        char* cpy = strdup(pwd); 
        pwd[0] = '~'; 
        strcpy(pwd+1, cpy+homeLength);      
    }//checking if the current directory is in the "home" directory of the user
    

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

    free(pwd);

    return prompt; 
}

void set_canonical_mode(int fd, int status){
    struct termios oldt; 
    tcgetattr(fd, &oldt); 
    if(status == 0)
        oldt.c_lflag &= ~(ICANON | ECHO); 
    else if (status == 1)
        oldt.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}