#include "../include/executor.h"
#include "../include/parser.h"
#include "../include/utils.h"
#include "../include/builtin.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>


int sigNum = -1; 

int execute(command command){

    int result = 0;   
    pid_t pid = fork(); 

    switch(pid){
        case -1 : //if the creation of the child process faild 
            result = -2; 
            break;

        case 0 : //child process
            set_canonical_mode(STDIN_FILENO, 1); //enabling canonical mode

            //If the redirection didn't fail
            if( (redirect_IO(command)) == 0){
                if(is_builtin(command.name) == 1)
                    result = execute_builtin(command); 
                else
                    result = execute_external(command); 

                if(result == -1)
                    result = EXIT_FAILURE; 
            }//If the redirection didn't fail

            else{
                result = -5; 
            }

            set_canonical_mode(STDIN_FILENO, 0); //enabling canonical mode
            exit(result);
            break;

        default : //parent process
            int status; 
            waitpid(pid, &status, 0); //waiting for the child process
            set_canonical_mode(STDIN_FILENO, 0); //disabling canonical mode

            if(WIFEXITED(status) != 0){
                int returnedValue = WEXITSTATUS(status);
                if( returnedValue == EXIT_FAILURE)
                    result = -1; 
                else 
                    result = returnedValue;
            }

            else if(WIFSIGNALED(status) != 0){
                sigNum = WTERMSIG(status); 
                result = -3; 
            }

            else if(WIFSTOPPED(status) != 0){
                sigNum = WSTOPSIG(status); 
                result = -4; 
            }

            break;
    }
    return result; 
}


int execute_builtin(command command){
    int result = 0; 

    if(strcmp(command.name, "cd") == 0){
        if (cd(command) == 0)
            result = REFRESH_PROMPT; // If cd is successful, tell the caller to refresh the prompt
        else
            result = -1; 
    }

    else if (strcmp(command.name, "pwd") == 0){
        result = pwd(); 
    }

    else if(strcmp(command.name, "exit") == 0){
        return(EXIT_COMMAND); 
    }

    else if(strcmp(command.name, "echo") == 0){
        for(int i = 0; i < command.argsNum ; i++)
            printf("%s ", command.args[i]);
        printf("\n");  
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

    else if(strcmp(command.name, "unset") == 0){
        unset(command);  
    }

    return result;
}


int execute_external(command command){

    char* executableName;
    char* slash; 
    int result = 0;

    //getting the pointer of the last slash in the command's name
    slash = strrchr(command.name, '/');

    if(slash == NULL){ //if there is no slash in the name
        executableName = command.name; 
    }
    else{
        executableName = slash+1; //the last file of the path is the executable
    }

    //array containing every arg and opt necessary for the 'execvp' function
    char* argsAndOpts[command.argsNum+2]; 
    argsAndOpts[0] = strdup(executableName);

    //filling the array with the arguments
    for(int i = 0 ; i < command.argsNum ; i++){
        argsAndOpts[i+1] = strdup(command.args[i]); 
    }

    //necessary for the 'execvp' function
    argsAndOpts[command.argsNum+1] = NULL;

    result = execvp(command.name, argsAndOpts); //executing the command
    if(result == -1){
        switch(errno){
            case EACCES : 
                perror("Vous n'avez pas les permissions nécessaires pour exécuter cette commande"); 
                break; 
            
            case EINVAL : 
                perror("Il manque le nom de l'exécutable"); 
                break; 
            
            case ENOENT :
                perror("Aucune commande existe avec ce nom"); 
                break; 
            
            case ENOMEM :
                perror("Mémoire insuffisante"); 
                break; 
            
            case ENOTDIR: 
                perror("Le chemin vers l'exécutable n'existe pas"); 
                break;

            default :
                perror("Une erreur s'est produite"); 
                break;  
        }
    }
    return result;
}


int redirect_IO(command command){

    if(command.outputRedirFile != NULL){

        //verifying if the file exists
        FILE* file = fopen(command.outputRedirFile, "r"); 
        //if it doesn't exist
        if(file == NULL && errno == ENOENT){
            //creating it
            file = fopen(command.outputRedirFile, "w"); 
            if(file == NULL){
                perror("Une erreur est surevenue lors de la redirection de la sortie standard"); 
                return -1;  
            }
        }
        if(file != NULL)
            fclose(file);

        int flag = O_WRONLY|O_CREAT; 
        if(command.appendRedirect == 1)
            flag |= O_APPEND; 
        else
            flag |= O_TRUNC;

        int outputFD = open(command.outputRedirFile, flag); 
        if(outputFD != -1)
            dup2(outputFD, STDOUT_FILENO);
        else{
            perror("Une erreur est surevenue lors de la redirection de la sortie standard"); 
            return -1;  
        }
    }

    if(command.inputRedirFile){
        int flag = O_RDONLY; 
        int inputFD = open(command.inputRedirFile, flag); 
        if(inputFD != -1)
            dup2(inputFD, STDIN_FILENO);
        else{
            perror("Une erreur est surevenue lors de la redirection de l'entrée standard"); 
            return -1;  
        }
    }

    return 0; 
}