/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Jude
 *
 * Created on February 21, 2021, 5:36 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * 
 */
int main(int argc, char** argv) {
    char *str = NULL;
    size_t count = 0;
    int str_len;
    char **tokens = NULL;
    int token_count = 0;

    if ((str_len = getline(&str, &count, stdin)) < 0) {
        printf("error1\n");
        return (EXIT_FAILURE);
    }
    str[str_len - 1] = 0;

    tokens = malloc(sizeof (char*)*count);

    int pipe_fd[2];

    if (pipe(pipe_fd) < 0) {
        printf("error2\n");
        return (EXIT_FAILURE);
    }

    tokens[token_count] = strtok(str, " ");
    while (tokens[token_count] != NULL) {
        if (strcmp(tokens[token_count], "|") == 0) {
            int pid = fork();
            if (pid < 0) {
                printf("error3\n");
                return (EXIT_FAILURE);
            } else if (pid > 0) {
                tokens[token_count] = NULL;
                if (dup2(pipe_fd[1], STDOUT_FILENO) < 0) {
                    printf("error4\n");
                    return (EXIT_FAILURE);
                }
                close(pipe_fd[0]);
                execvp(tokens[0], tokens);
            } else {
                token_count = -1;
            }
        }
        tokens[++token_count] = strtok(NULL, " ");
    }

    if (dup2(pipe_fd[0], STDIN_FILENO) < 0) {
        printf("error5\n");
        return (EXIT_FAILURE);
    }
    close(pipe_fd[1]);
    execvp(tokens[0], tokens);

    free(tokens);
    free(str);

    return (EXIT_SUCCESS);
}

