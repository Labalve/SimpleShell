#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <time.h>
#include "helpers.c"


char * inputHandler;
char inputParamHandler[10][10];
int paramNumber = 0;

void exitShell()
{
    if(paramNumber > 0){
        exit(atoi(inputParamHandler[0]));
    }
    else{
        exit(0);
    }
}

void lsShell()
{
    char dateBuffer[100];
    char cwd[1024];
    DIR *givenDirectory;
    struct dirent *currentFile;
    struct stat fileStat;
    bool withDetails = false;
    bool withHidden = false;
    if(paramNumber > 0){
        if(strcmp(inputParamHandler[0],"-l") == 0) withDetails = true;
        else if(strcmp(inputParamHandler[0],"-a") == 0) withHidden = true;
        if(paramNumber > 1){
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
        if(withHidden == false)
        {
            if(startsWith(currentFile->d_name, ".")) continue;
        }
        stat(currentFile->d_name, &fileStat);
        if(withDetails)
        {
            char dateBuffer[80];
            struct tm tm;
            int epochTime = fileStat.st_mtime;
            struct tm * timeinfo;
            time_t epochTimeAsTimeT = epochTime;
            timeinfo = localtime(&epochTimeAsTimeT);
            strftime(dateBuffer, 80, "%Y %b %d %H:%M", timeinfo);

            struct passwd *pw = getpwuid(fileStat.st_uid);

            printFileMode(fileStat);

            printf("\t%s\t%d\t%s\t", pw->pw_name, fileStat.st_size, dateBuffer);
        }
        printf(" %s", currentFile->d_name);
        if(withDetails) printf("\n");
    }
    closedir(givenDirectory);
    printf("\n");
}

void cdShell()
{
    if(paramNumber > 0){
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
    char * longHelp = "This is Simple Shell application created by Krzysztof Kulak as a university project.\nIt implements basic shell functionality.\n\ncd <PATH> - change directory to path given in <PATH> parameter\n\nexit <STATUS> - ends Shell process and return 0 or number given in <STATUS> parameter\n\nhelp <FLAG> - displays help message. Command with -l flag shows longer help message.\n\nls <FLAG> - lists files and catalogs in the current directory.\n\tFlag -a shows hidden files.\n\tFlag -l shows extended list view in format: <file mode> <size> <owner> <date of the last modification> <name>\n\nhead <PATH> <FLAG> - shows first n lines of file with path provided in <PATH>. n is equal 10, unless flag -n and value provided.";
    if(paramNumber > 0){
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

void headShell()
{
    int num, pos, count;
    FILE *fp;
    char (*array)[4096];
    fp = fopen(inputParamHandler[0], "r");
    if (fp == NULL) {
        printf("Cannot open file\n");
    } else {
        if(paramNumber >= 3){
            if(strcmp(inputParamHandler[1], "-n") == 0)
            {
                num = atoi(inputParamHandler[2]);
            }
        } else {
            num = 10;
        }
        array = malloc(4096 * (num + 1));
        for (count = pos = 0; fgets(array[pos], 4096, fp) != NULL; count++) {
            if (count < num)
                fputs(array[pos], stdout);
            if (++pos >= num + 1)
                pos = 0;
        }
        if (count > num) {
            pos = count - num;
            if (pos > num) {
                printf("...\n");
            }
        }
        fclose(fp);
    }
}

int otherCommand()
{
    char finalCommand[120];
    strcat(finalCommand, inputHandler);
    for(int i = 0; i < paramNumber; i++){
        strcat(finalCommand, " ");
        strcat(finalCommand, inputParamHandler[i]);
    }
    return system(finalCommand);
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
             paramNumber++;
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
    else if(strcmp(inputHandler, "cd") == 0){
        cdShell();
    }
    else if(strcmp(inputHandler, "help") == 0){
        helpShell();
    }
    else if(strcmp(inputHandler, "ls") == 0){
        lsShell();
    }
    else if(strcmp(inputHandler, "head") == 0){
        headShell();
    }
    else {
        otherCommand();
    }
}

void clearGlobalVars()
{
    memset(inputHandler,0,sizeof(inputHandler));
    memset(inputParamHandler,0,sizeof(inputParamHandler));
    paramNumber = 0;
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
