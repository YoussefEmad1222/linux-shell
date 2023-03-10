#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
void shell_init() // is to print the welcome message
{
    char *username = getenv("USER");
    system("clear");
    printf("***********************************************************");
    printf("\n\n\t****MY SHELL****");
    printf("\nWelcome to My Shell\n");
    printf("\nYour username is %s\n", username);
    printf("\n***********************************************************\n");
    printf("\n\n");
    sleep(3);
    system("clear");
    
}

void openHelp() // is to print the help menu
{
    puts("\n***WELCOME TO MY SHELL HELP***"
         "\n\n\t-USE AT YOUR OWN RISK-"
         "\nList of Commands supported:"
         "\n>cd"
         "\n>>export"
         "\n>>echo"
         "\n>ls"
         "\n>mkdir"
         "\n>rm"
         "\n>cat"
         "\n>pwd"
         "\n>exit"
         "\n>help"
         "\n>all other general commands available in UNIX shell"
         "\n>improper space handling"
         "\n>improper command handling"
         "\n>improper argument handling(cd,export and echo not all arguments possibilites are handled,be careful)");
    return;
}
// setup the environment
void setup_environment()
{
    chdir(getenv("HOME"));
}
// register the child signal to do the reap_child function and write_log_file function
void register_child_signal(void (*on_child_exit)(int))
{
    signal(SIGCHLD, on_child_exit);
}
// routine to reap the child process and write the log file to log.log
// if the child process is terminated by a signal, then the log file will be written with the signal number
void on_child_exit()
{
    reap_child();
    write_log_file("child exited\n");
}
// is to tell the parent process to reap the child process and teminate the child process
void reap_child()
{
    while (waitpid((pid_t)-1, NULL, WNOHANG) > 0)
        ;
}
// write the log file to log.
void write_log_file(char *msg)
{
    FILE *fp;
    fp = fopen("/home/youssef/Desktop/log.log", "a");
    if (fp == NULL)
    {
        printf("log file not found\n");
        return;
    }
    fprintf(fp, "%s\n", msg);
    fclose(fp);
}
// is to read the input from the user
void takes_input(char *cmd)
{
    fgets(cmd, 100, stdin);
    cmd[strlen(cmd) - 1] = '\0';
}
// is to parse the input from the user
void parse(char *cmd, char *args[])
{
    int i = 0;
    char *token, *token2;
    int len = strlen(cmd);
    char *ptr = strchr(cmd, '"');
    if (ptr != NULL)
    {
        strcpy(ptr, ptr + 1);
        cmd[strlen(cmd) - 1] = '\0';
    }
    token = strtok(cmd, " ");
    while (token != NULL)
    {
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}
// is to evaluate the input from the user if there are some environment variables
void evaluate(char *args[])
{
    for (int i = 0; args[i] != NULL; i++)
    {
        if (*args[i] == '$')
        {
            char *env;
            *args[i] = '\n';
            env = getenv(args[i] + 1);
            args[i] = env;
        }
    }
}
// is to check the input from the user if it is a builtin command or a normal command
int check_input(char *args[])
{
    int input_type = 0;
    if (!strcmp(args[0], "cd") || !strcmp(args[0], "echo") || !strcmp(args[0], "export"))
    {
        input_type = 1;
    }
    else
    {
        input_type = 2;
    }
    return input_type;
}
// is to compose the arguments to be passed to the execvp function if the enviroment variables are passed
void compose_args(char *args[], char *coArgs[])
{
    char *token, *token2;
    int i = 1;
    token = strtok(args[1], " ");
    coArgs[0] = args[0];
    for (int i = 1; args[i] != NULL; i++)
    {
        token = strtok(args[i], " ");
        while (token != NULL)
        {
            coArgs[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
    }
}
// is to execute the cd command
void cd_command(char *args[])
{
    char *cd;
    if (args[1] == NULL || !strcmp(args[1], "~")) // if the user didn't pass any argument or passed ~
    {
        cd = getenv("HOME");
        chdir(cd);
        printf("%s\n", cd);
    }
    else if (!strcmp(args[1], "..")) // if the user passed ..
    {
        char pwd[1000] = {};
        strcpy(pwd, getenv("PWD"));
        char *last_token = strrchr(pwd, '/');
        *last_token = '\0';

        chdir(pwd);
        printf("%s\n", pwd);
    }
    else if (!strcmp(args[1], ".")) // if the user passed .
    {
        cd = getenv("PWD");
        chdir(getenv("PWD"));
        printf("%s\n", cd);
    }
    else // if the user passed a directory
    {
        cd = args[1];
        printf("%s\n", cd);
    }
    if (chdir(cd)) // if the directory doesn't exist
    {
        perror("cd");
        usleep(1000000);
        return;
    }
}
// is to execute the export command and set or update the environment variable
void export_command(char *args[])
{

    char *env;
    
    char str[100] = "";
    /*
    if the user passed a string with spaces, then the string will be concatenated to one string 
    and the spaces will be replaced by a single space
    then the string will be in args[1]
    then it will used to set the environment variable
    */ 
    for (int i = 1; args[i] != NULL && i < 100; i++)
    {
        strcat(str, args[i]);
        strcat(str, " ");
    }
    args[1] = str;
    strcat(args[1], "\0");
    env = strtok(args[1], "="); // is to get the environment variable name
    char *env2 = strtok(NULL, "="); // is to get the environment variable value
    if (env2 == NULL) // if the user didn't pass a value
    {
        printf("export: not enough arguments\n");
        return;
    }
    setenv(env, env2, 1);// set environment variable
}
// is to execute the echo command and print the arguments
void echo_command(char *args[])
{
    for (int i = 1; args[i] != NULL; i++)
    {
        printf("%s ", args[i]);
    }
    printf("\n");
}
// is to execute the builtin commands cd, export, echo
void execute_builtin(char *args[])
{
    if (!strcmp(args[0], "cd")) // is to check if it is the cd command
    {
        cd_command(args);
    }
    else if (!strcmp(args[0], "echo")) // is to check if it is the echo command
    {
        echo_command(args);
    }
    else if (!strcmp(args[0], "export")) // is to check if it is the export command
    {
        export_command(args);
    }
}
// is to execute the normal commands
void execute(char *args[])
{
    pid_t pid;
    pid = fork(); // forking the process
    if (pid == 0) // child process
    {
        char *coArgs[100] = {};
        // if the user wants to run the process in the background
        // we remove the & from the arguments to run the process in the background
        if (args[1] != NULL && !strcmp(args[1], "&"))
        {
            args[1] = NULL;
        }
        compose_args(args, coArgs);          // compose the arguments to be passed to the execvp function if the enviroment variables are passed
        if (execvp(coArgs[0], coArgs) == -1) // if the command is not found
        {
            printf("command not found: %s\n", args[0]);
            exit(0);
        }
    }
    else if (pid < 0) // error in forking
    {
        printf("error in forking\n");
    }
    else // parent process
    {
        // if the user wants to run the process in the background we don't wait for the child processes to finish
        if (args[1] != NULL && !strcmp(args[1], "&"))
        {
            return;
        }
        waitpid(pid, 0, 0); // wait for the child process to finish
    }
}
void shell()
{
    int run = 1;
    do
    {
        char cmd[100] = {};   // the command that the user will enter
        char *args[100] = {}; // the commands and the arguments of the command
        printf("\n>>>> ");
        int input_type = 0;           // the type of the input
        takes_input(cmd);             // takes input from user;
        parse(cmd, args);             // parse the input
        evaluate(args);               // evaluate the input
        input_type = check_input(args); // check the input type
        if (!strcmp(args[0], "exit")) // check if the user wants to exit the shell
        {
            run = 0;
            continue;
        }
        else if (!strcmp(args[0], "help")) // check if the user wants to open the help of the shell
        {
            input_type = 3;
        }
        if (input_type == 1)
        {
            execute_builtin(args); // execute a builtin command
        }
        else if (input_type == 2)
        {
            execute(args); // execute a non builtin command
        }
        else
        {
            openHelp(); // open the help of the shell for the user
        }
    } while (run == 1);
}

int main()
{
    shell_init();
    register_child_signal(on_child_exit);
    setup_environment();
    shell();
    return 0;
}
