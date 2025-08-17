#include <stdio.h>
#include <stdlib.h> 
#include "./include/utils.h"
#include "./include/parser.h"


int main(void){

    char* prompt = get_prompt();
    char* input;
    command command; 

    while(1){
        printf("%s ", prompt);
        if(!input)
            free(input);
        input = read_input();
        command = parse_command(input); 
    }

}

