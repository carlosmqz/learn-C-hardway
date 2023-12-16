#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 10
#define LOG_FOLDER "~/.logfind"

int find_tokens(int arg_o,int arg_num, char *tokens_to_find[])
{
    int i = 0;
    
    for(i = 0; i < arg_num; i++){
        printf("%s\n", tokens_to_find[i]);
    }
    
    return 0;
}
int main(int argc, char *argv[])
{
    int arg_o = 0;
    int result = 0;
    int k = 0;
    int i = 0;
    char *tokens_to_find[MAX_TOKENS];
    printf("Arguments number: %d\n", argc);
    if (argc == 1){
        printf("Please introduce the text to search\n");
        return -1;
    }
    if(argc > 1)
    {
        if(strcmp(argv[1],"-o") == 0){
            arg_o = 1;
            printf("Flag -o active\n");
        }
        for(i = 1; i < argc && k < MAX_TOKENS-1; i++)
        {
            if(strcmp(argv[i], "-o") != 0)
            {
                tokens_to_find[k] = malloc(sizeof(char *) * 100);
                tokens_to_find[k] = argv[i];
                k++;
            }
        }
        result = find_tokens(arg_o, k, tokens_to_find);

    }
    
    return 0;
}
