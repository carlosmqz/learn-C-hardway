#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>
#define MAX_TOKENS 10



int find_tokens(int arg_o,int arg_num, char *tokens_to_find[])
{
    int i = 0;
    char *buffer = NULL;  
    int tokens_found = 0;
    glob_t glob_buff;
    
    glob("logs/*.log", GLOB_MARK, NULL, &glob_buff);
    for(size_t path_index = 0; path_index < glob_buff.gl_pathc; path_index++){
    
        FILE *file = fopen(glob_buff.gl_pathv[path_index],"r");
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
                int i = 0;
                for(i = 0; i < arg_num; i++){
                    if(strstr(buffer, tokens_to_find[i]) != NULL){
                        //printf("Token %s was found in the file! \n", tokens_to_find[i]);
                        tokens_found++;
                    }                
                }
            }
            fclose(file);
        }
        if(arg_o == 0 && tokens_found == arg_num){
            printf("All tokens were found in file: %s\n", glob_buff.gl_pathv[path_index]);
        }else if(arg_o == 1 && tokens_found <= arg_num && tokens_found != 0){
            printf("At least %d tokens found in file: %s\n", tokens_found, glob_buff.gl_pathv[path_index]);
        }
        tokens_found = 0;
        free(buffer);
    }
    globfree(&glob_buff);
    return 0;
}
int main(int argc, char *argv[])
{
    int k = 0;
    int i = 0;
    char *tokens_to_find[MAX_TOKENS];
    int arg_o = 0;
    if (argc == 1){
        printf("Please introduce the text to search\n");
        return -1;
    }
    if(argc > 1)
    {
        for(int i=1; i < argc; i++){
            char c = argv[i][0];
            switch (c) {
                case '-':
                    char argument = argv[i][1];
                    switch(argument)
                    {
                        case 'o': 
                            arg_o = 1;
                            break;
                        default:
                            printf("Invalid argument option.");
                            break;
                    }
                break;
                default:
                    tokens_to_find[k] = malloc(sizeof(char *) * 100);
                    tokens_to_find[k] = argv[i];
                    k++;
                    break;
            }
        }
        find_tokens(arg_o, k, tokens_to_find);
    }
    return 0;
}
