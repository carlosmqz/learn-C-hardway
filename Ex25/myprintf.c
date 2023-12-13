#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "dbg.h"

int print_format(const char *str, ...)
{
    va_list vargs;
    va_start(vargs, str);

    char token[1000];
    int k =0;

    for(int i = 0; str[i] != '\0'; i++){
        token[k++] = str[i];
        if(str[i+1] == '%' || str[i+1] == '\0'){
            token[k] = '\0';
            k = 0;
            if(token[0] != '%'){
                fprintf(stdout, "%s", token);
            }else{
                int j = 1;
                char ch1 = 0;
                while((ch1 = token[j++]) < 58) {}
                if(ch1 == 'i' || ch1 == 'd' || ch1 == 'u' || ch1 == 'h'){
                        fprintf(stdout, token, va_arg(vargs, int));
                }
                else if(ch1 == 'c'){
                    fprintf(stdout, token, va_arg(vargs, int));
                }else if(ch1 == 'f'){
                    fprintf(stdout, token, va_arg(vargs, double));
                }else if(ch1 == 'l'){
                    char ch2 = token[2];
                    if(ch2 == 'i' || ch2 == 'u' || ch2 == 'd'){
                        fprintf(stdout, token, va_arg(vargs, long));
                    }else if(ch2 == 'f'){
                        fprintf(stdout, token, va_arg(vargs, double));
                    }
                }else if(ch1 == 'L'){
                    char ch2 = token[2];
                    if(ch2 == 'u' || ch2 == 'd' || ch2 == 'i'){
                        fprintf(stdout, token, va_arg(vargs, long long));
                    }else if(ch2 == 'f'){
                        fprintf(stdout, token, va_arg(vargs, long double));
                    }
                }else if(ch1 == 's'){
                    fprintf(stdout, token, va_arg(vargs, char *));
                }else{
                    fprintf(stdout, "%s", token);
                }
            }
        }
    }
    va_end(vargs);
    return 0;
}

int main(int argc, char *argv[])
{
    //char *message = NULL;

    print_format("Hello World, this is %s and I'm learning C programming, this is day %d","Carlos", 975);
    return 0;

}
