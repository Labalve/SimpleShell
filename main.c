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

void cdBash()
{
    if(ParamNumber > 0){
        if(chdir(inputParamHandler[0]) == -1){
            printf("wrong path provided\n");
        };
    }
    else{
        printf("no path provided\n");
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
        printf("error occur red while setting working directory");
        return 1;
    }
}

int getInput()
{
    char input[100];
    fgets(input, 100, stdin);
    strchr(input, '\n');
    char *pos;
    if ((pos=strchr(input, '\n')) != NULL)
        *pos = '\0';
    const char s[2] = " ";
    char *token;
    token = strtok(input, s);
    int i = 0;
    while( token != NULL )
    {
         if(i == 0) inputHandler = token;
         else {
             strcpy(inputParamHandler[(i-1)], token);
             ParamNumber++;
         }
         token = strtok(NULL, s);
         i++;
    }
    if(strcmp(input, "") == 0) return 0;
    else return 1;
}

void actionManager()
{
    if(strcmp(inputHandler, "exit") == 0){
        exitBash();
        printf("\n");
    }
    if(strcmp(inputHandler, "cd") == 0){
        cdBash();
    }

}

void clearGlobalVars()
{
    memset(inputHandler,0,sizeof(inputHandler));
    memset(inputParamHandler,0,sizeof(inputParamHandler));
    ParamNumber = 0;
}

int main()
{
    welcomeScreen();
    while(1){
        if(printPrompt() == 1){ return 1; }
        while(getInput() == 0){
            printPrompt();
        };

        actionManager();

    }

    return 0;
}
