#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 10
#define LOG_FOLDER "~/.logfind"



int find_tokens(int arg_o,int arg_num, char *tokens_to_find[])
{
    int i = 0;
    char *buffer = NULL;  
    int tokens_found = 0;

    FILE *file = fopen("input.txt","r");
    if (file == NULL){
        printf("Error reading the file");
        return 1;
    }

    if(fseek(file, 0L, SEEK_END) == 0){
        long bufsize = ftell(file);
        if(bufsize == -1){
            printf("Error reading the file, buffer size");
            return 1;
        }

        buffer = malloc(sizeof(char *) * (bufsize+1));
        if(fseek(file, 0L, SEEK_SET) != 0){
            printf("Error getting the start of the file");
            return 1;
        }

        size_t newLen = fread(buffer, sizeof(char), bufsize, file);
        if(ferror(file) != 0){
            fputs("Error reading the file %s", stderr);
        }else{
            buffer[newLen++] = '\0';
            printf("Buffer: %s\n", buffer);
            int i = 0;
            for(i = 0; i < arg_num; i++){
                if(strstr(buffer, tokens_to_find[i]) != NULL){
                    printf("Token %s was found in the file! \n", tokens_to_find[i]);
                    tokens_found++;
                }else{
                    printf("Token %s was not found in the file!\n", tokens_to_find[i]);
                }
            }
        }
        fclose(file);
    } 
    
    if(tokens_found != 0)
        printf("%d tokens were found in the file!\n", tokens_found);
    free(buffer);
    return 0;
}
int main(int argc, char *argv[])
{
    int result = 0;
    int k = 0;
    int i = 0;
    char *tokens_to_find[MAX_TOKENS];
    int arg_o = 0;
    printf("Arguments number: %d\n", argc);
    if (argc == 1){
        printf("Please introduce the text to search\n");
        return -1;
    }
    if(argc > 1)
    {
        for(int i=1; i < argc; i++){
            char c = argv[i][0];
            printf("%c\n", c);
            switch (c) {
                case '-':
                    char argument = argv[i][1];
                    switch(argument)
                    {
                        case 'o': 
                            printf("OR option enabled\n");
                            arg_o = 1;
                            break;
                        default:
                            printf("Invalid argument option.");
                    }
                break;
                default:
                    tokens_to_find[k] = malloc(sizeof(char *) * 100);
                    tokens_to_find[k] = argv[i];
                    k++;
            }
        }
        result = find_tokens(arg_o, k, tokens_to_find);
    }
    return 0;
}
