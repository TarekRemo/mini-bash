#include <stdio.h>
#include "./include/utils.h"


int main(void){

    char* prompt = get_prompt(prompt);
    char* input; 

    while(1){
        printf("%s ", prompt);
        input =read_command(); 
    }

}

