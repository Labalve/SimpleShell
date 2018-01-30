#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <pwd.h>
#include <time.h>


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

void printFileMode(struct stat fileStat){
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
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
    char * longHelp = "This is Simple Shell application created by Krzysztof Kulak as a university project.\nIt implements basic shell functionality.\n\ncd <PATH> - change directory to path given in <PATH> parameter\n\nexit <STATUS> - ends Shell process and return 0 or number given in <STATUS> parameter\n\nhelp <FLAG> - displays help message. Command with -l flag shows longer help message.\n\nls <FLAG> - lists files and catalogs in the current directory.\n\tFlag -a shows hidden files.\n\tFlag -l shows extended list view in format: <file mode> <size> <owner> <date of the last modification> <name>\n";
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

void tailShell()
{
    FILE *in;
    int count = 0;
    long int pos;
    char s[100];
    in = fopen(inputHandler, "r");
    if (in == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fseek(in, 0, SEEK_END);
    pos = ftell(in);
    while (pos) {
        fseek(in, --pos, SEEK_SET);
        if (fgetc(in) == '\n') {
            if (count++ == 10) break;
        }
    }
    while (fgets(s, sizeof(s), in) != NULL) {
        printf("%s", s);
    }
    fclose(in);
    return 0;
}

int otherCommand()
{
    char finalCommand[120];
    strcat(finalCommand, inputHandler);
    for(int i = 0; i < ParamNumber; i++){
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
    else if(strcmp(inputHandler, "cd") == 0){
        cdShell();
    }
    else if(strcmp(inputHandler, "help") == 0){
        helpShell();
    }
    else if(strcmp(inputHandler, "ls") == 0){
        lsShell();
    }
    else if(strcmp(inputHandler, "tail") == 0){
        tailShell();
    }
    else {
        otherCommand();
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
