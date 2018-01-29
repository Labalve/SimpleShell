#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char * inputHandler;
char inputParamHandler[10][10];
int ParamNumber = 0;

void welcomeScreen()
{
    printf("\n\t==========C-Shell==========\n");
    printf("\t==author: Krzysztof Kulak==\n");
    printf("\t===========================\n\n");
}

void exitBash()
{
     if(ParamNumber > 0){
            exit(atoi(inputParamHandler[0]));
        }
        else{
            exit(0);
        }
}

int printPrompt()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        fprintf(stdout, "[%s] ", cwd);
        return 0;
    }
    else {
        printf("error occured while setting working directory");
        return 1;
    }
}

void getInput()
{
    char input[100];
    scanf ("%[^\n]%*c", input);
    const char s[2] = " ";
    char *token;
    token = strtok(input, s);
    int i = 0;
    while( token != NULL ) {
        if(i == 0) inputHandler = token;
        else {
            strcpy(inputParamHandler[(i-1)], token);
            ParamNumber++;
        }
        token = strtok(NULL, s);
        i++;
   }
}

void actionManager()
{
    if(strcmp(inputHandler, "exit") == 0){
        exitBash();
        printf("\n");
    }
    memset(inputHandler,0,sizeof(inputHandler));
    memset(inputParamHandler,0,sizeof(inputParamHandler));
}

int main()
{
    welcomeScreen();
    while(1){
        if(printPrompt() == 1){
            return 1;
        };
        getInput();
    //printf("%s", inputHandler);

        actionManager();
    }

/*    printf("%s", inputParamHandler[0]);
    printf("%s", inputParamHandler[1]);
    printf("%s", inputParamHandler[2]);*/
    return 0;
}
