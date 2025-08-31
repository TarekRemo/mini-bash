#include "./include/utils.h"
#include "./include/parser.h"
#include "./include/builtin.h"
#include "./include/executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main(void){

    char* prompt = get_prompt();
    load_commands_history(); 

    char* input = NULL;
    command command; 
    int result; 

    while(1){
        printf("%s ", prompt);
        if(input)
            free(input);
        input = read_input();

        command = parse_command(input); 
        if(errno == UNEXPECTED_CHARACTER){
            printf("Un caractère inattendu est présent\n"); 
            continue; 
        } 

        result = execute(command); 
        switch(result){
            case EXIT_COMMAND : 
                free(prompt); 
                free(input); 
                set_canonical_mode(STDIN_FILENO, 1); //reactivating the canonical mode for the terminal
                exit(EXIT_SUCCESS); 
                break;
            case REFRESH_PROMPT:
                free(prompt); 
                prompt = get_prompt(); 
                break; 
            case -1 : 
                printf("Impossible d'exécuter la commande. Une erreur est survenue\n"); 
                break;
            case -2 : 
                printf("impossible de créer un processus fils pour exécuter la commande\n"); 
                break;
            case -3: 
                printf("Le processus fils a été terminé par le signal : %d\n", sigNum);
                break; 
            case -4: 
                printf("Le processus fils a été arrêté par le signal : %d\n", sigNum);  
                break; 
        }
                    
    }
}

