/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: jd936
 *
 * Created on February 7, 2021, 6:10 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 * 
 */
int main(int argc, char** argv) {
    int ch;
    char command[101];
    int fork_id;
    char *args[101];
    args[0] = command;
    
    do  {
        int i = 0;
        int arg_counter = 1;
        int first = -1;
       
        printf("Your command>");

        while ( (ch = getchar()) != EOF && ch != '\n') {
            if (ch == ' ') {
                if (first < 0) {
                    first = i;
                }
                command[i++] = 0;
                args[arg_counter++] = command + i;
            }
            else {
                command[i++] = ch;
            }
        }
        if (i == 0){
            printf("invalid input");
            return EXIT_FAILURE;
        }
        if (first < 0){
            first = i;
        }
        command[i] = 0;
        args[arg_counter] = NULL;
        fork_id = fork();
        if (fork_id < 0) {
            printf("error!");
            return EXIT_FAILURE;
        }
        else if (fork_id == 0) {
            if (command[first-1] == '&')
            {
                command[first-1] = 0;
            }
            execvp(args[0],args);
        }
        else {
            if (command[first-1] != '&')
            {
                wait(NULL);
            }
        }
    }
    while (ch != EOF);
    return (EXIT_SUCCESS);
}

