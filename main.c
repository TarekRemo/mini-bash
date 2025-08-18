#include <stdio.h>
#include <stdlib.h>
#include "./include/utils.h"
#include "./include/parser.h"


int main(void){

    char* prompt = get_prompt();
    historyCommands = malloc(0); // Initializing the historyCommands array 
    nbHistoryCommands = 0; // Initializing the number of history commands

    load_commands_history(); 
    char* input = NULL;
    command command; 

    while(1){
        printf("%s ", prompt);
        if(input)
            free(input);
        input = read_input();
        command = parse_command(input); 
    }

}

