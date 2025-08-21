#include "../include/executor.h"
#include "../include/parser.h"
#include "../include/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

int execute(command command){

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
    char* argsAndOpts[command.argsNum+command.optionsNum+2]; 
    argsAndOpts[0] = strdup(executableName);

    //filling the array with the arguments
    for(int i = 0 ; i < command.argsNum ; i++){
        argsAndOpts[i+1] = strdup(command.args[i]); 
    }

    //filling the array with the options
    for(int i = 0 ; i < command.optionsNum ; i++){
        argsAndOpts[command.argsNum+1+i] = strdup(command.options[i]); 
    }

    //necessary for the 'execvp' function
    argsAndOpts[command.argsNum+command.optionsNum+1] = NULL;  

    pid_t pid = fork(); 

    switch(pid){
        case -1 : //if the creation of the child process faild 
            perror("impossible de créer un processus fils pour exécuter la commande"); 
            result = -2; 
            break;

        case 0 : //child process
            set_canonical_mode(STDIN_FILENO, 1); //enabling canonical mode
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
                return EXIT_FAILURE; 
            }
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
                printf("Le processus fils a été terminé par le signal : %d\n", WTERMSIG(status)); 
                result = -3; 
            }

            else if(WIFSTOPPED(status) != 0){
                printf("Le processus fils a été arrêté par le signal : %d\n", WSTOPSIG(status)); 
                result = -4; 
            }

            break;
    }
    return result; 
}