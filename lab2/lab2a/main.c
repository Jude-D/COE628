#include <stdio.h>  //printf, fprintf
#include <stdlib.h> //EXIT_SUCCESS
#include <string.h> //strlen, strcmp
#define TOO_MANY_ARGS 2
#define TOO_FEW_ARGS 1

void strcat_substr(char *dest, char *src, int start_ind, int end_rev_ind) {
    int dest_len = strlen(dest);
    int src_len = strlen(src);
    int range_size = src_len - end_rev_ind - start_ind;
    memcpy(dest + dest_len, src + start_ind, range_size);
    dest[dest_len + range_size] = 0;
}

char *getarg(char *token) {
    char *arg = (char *) malloc(256);
    arg[0] = 0;
    if (token[0] == '"') {
        if (token[strlen(token) - 1] == '"') {
            strcat_substr(arg, token, 1, 1);
            return arg;
        } else {
            strcat_substr(arg, token, 1, 0);
            while (token = strtok(NULL, " ")) {
                strcat_substr(arg, " ", 0, 0);
                if (token[strlen(token) - 1] == '"') {
                    strcat_substr(arg, token, 0, 1);
                    break;
                } else {
                    strcat_substr(arg, token, 0, 0);
                }
            }
            return arg;
        }
    } else {
        strcat_substr(arg, token, 0, 0);
        return arg;
    }
}

int main(int argc, char* argv[]) {
    //Default values:
    int exit_code = EXIT_SUCCESS;
    char * greeting = "Hello";
    char * person = "UNKNOWN";
    char *input;
    char *token;
    char *name;
    int size = 0;
    //Add code to change the exit_code depending on argc
    if (argc == 1) {
        exit_code = 0;
    } else if (argc == 2) {
        exit_code = 1;
    } else if (argc > 1) {
        exit_code = 2;
    }
    //Add code to change 'person' if one is given on command line
    int state = 0;
    while (getline(&input, &size, stdin)) {
        input[size - 1] = 0;
        token = strtok(input, " ");
        while (token) {
            if (state == 0) {
                if (strcmp(token, "hello") == 0) {
                    state = 1;

                } else if (strcmp(token, "bye") == 0) {
                    state = 2;
                } else {
                    break;
                }
            } else if (state == 1) {
                char * arg = getarg(token);
                printf("Hello %s \n", arg);
                free(arg);
                state = 3;
            } else if (state == 2) {
                char * arg = getarg(token);
                printf("Bye %s \n", arg);
                free(arg);
                state = 3;
            } else if (state == 3) {
                if (strcmp(token, "&&") == 0) {
                    state = 0;
                } else {
                    break;
                }
            }
            token = strtok(NULL, " ");
        }
        if (state == 1) {
            printf("Hello UNKNOWN \n");
        } else if (state == 2) {
            printf("Bye UNKNOWN \n");
        }
        size = 0;
    }


    int len = strlen(argv[0]);
    char * last3 = argv[0] + len - 3; //last3 points to last 3 chars
    fprintf(stderr, "%s\n", argv[0]);
    fprintf(stderr, "%s\n", last3);

    //Add code to change 'greeting' if last 3 chars of command are 'bye'

    printf("%s %s\n", greeting, person);

    fprintf(stderr, "exit_code: %d\n", exit_code);

    return exit_code; //Could also say exit(exit_code)
}