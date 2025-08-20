#include "./include/utils.h"
#include "./include/parser.h"
#include "./include/builtin.h"
#include <stdio.h>
#include <stdlib.h>

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

        if(is_builtin(command.name) == 1){
            result = execute_builtin(command); 
            if(result  == REFRESH_PROMPT ){
                free(prompt); 
                prompt = get_prompt(); 
            }
        }      
    }
}

