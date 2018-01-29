#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>

char * inputHandler;
char inputParamHandler[10][10];
int ParamNumber = 0;

void welcomeScreen()
{
    printf("\n\t==========C-Shell==========\n");
    printf("\t==author: Krzysztof Kulak==\n");
    printf("\t===========================\n\n");
}

bool startsWith(const char *haystack, const char *needle)
{
   if(strncmp(haystack, needle, strlen(needle)) == 0) return 1;
   return 0;
}

void exitShell()
{
    if(ParamNumber > 0){
        exit(atoi(inputParamHandler[0]));
    }
    else{
        exit(0);
    }
}

void lsShell()
{
    char cwd[1024];
    DIR *givenDirectory;
    struct dirent *currentFile;
    struct stat fileStat;
    bool withDetails = false;
    bool withHidden = false;
    if(ParamNumber > 0){
        if(strcmp(inputParamHandler[0],"-l") == 0) withDetails = true;
        else if(strcmp(inputParamHandler[0],"-a") == 0) withHidden = true;
        if(ParamNumber > 1){
            if(strcmp(inputParamHandler[1],"-l") == 0){
                withDetails = true;
            }
            else if(strcmp(inputParamHandler[1],"-a") == 0){
                withHidden = true;
            }
        }
    }
    givenDirectory = opendir(getcwd(cwd, sizeof(cwd)));
    while((currentFile = readdir(givenDirectory)) != NULL)
    {
        if(withHidden == false){
            if(startsWith(currentFile->d_name, ".")) continue;
        }
        stat(currentFile->d_name, &fileStat);
        if(withDetails)printf("%d\t",fileStat.st_size);
        printf(" %s", currentFile->d_name);
        if(withDetails) printf("\n");
    }
    closedir(givenDirectory);
    printf("\n");
}

void cdShell()
{
    if(ParamNumber > 0){
        if(chdir(inputParamHandler[0]) == -1){
            printf("wrong or inaccessible path provided\n");
        };
    }
    else{
        printf("no path provided\n");
    }
}

void helpShell()
{
    char * shortHelp = "This is Simple Shell application created by Krzysztof Kulak as a university project.\nIt implements basic shell functionality.\nUse -l flag to get longer help message.";
    char * longHelp = "This is Simple Shell application created by Krzysztof Kulak as a university project.\nIt implements basic shell functionality.\n\ncd <PATH> - change directory to path given in <PATH> parameter\nexit <STATUS> - ends Shell process and return 0 or number given in <STATUS> parameter\nhelp <FLAG> - displays help message. Command with -l flag shows longer help message.";
    if(ParamNumber > 0){
        if(strcmp(inputParamHandler[0],"-l") == 0){
            printf("%s", longHelp);
        }
    }
    else printf(shortHelp);
    printf("\n");
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
        exitShell();
        printf("\n");
    }
    if(strcmp(inputHandler, "cd") == 0){
        cdShell();
    }
    if(strcmp(inputHandler, "help") == 0){
        helpShell();
    }
    if(strcmp(inputHandler, "ls") == 0){
        lsShell();
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
        clearGlobalVars();
    }

    return 0;
}
