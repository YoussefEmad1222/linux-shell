# simple shell (multi-processing)

## Description

Simple-shell is a C-based implementation of a UNIX shell. Users can interact with the operating system using a simple but efficient command-line tool given by the shell. The Simple-shell implementation offers both built-in commands and the potential to run third-party software..

##. code Design
A parser, handler, cd command, export command, echo command, and main are among the functions in the simple-shell code.

The parser function parses a user-entered command into an array of arguments that can be passed to other functions. The terminal allows for a maximum of ten arguments.

When the child process terminates, the handler function is in charge of handling SIGCHLD signals. The parent process deletes the signal-initiating process from the processes table. In the case of a zombie process, this method is in charge of reaping it.

The cd command function is a built-in command that handles user-supplied commands to traverse between directories. The function allows the user to navigate to the home directory, one level up, or the previous directory, or a specific directory you want to choose.

The export command function is a built-in command for identifying and using environment variables in child processes.

The echo command function is a built-in command that prints the input line as a string or the value of a given environment variable.

The shell function is in charge of managing user input, parsing commands, and executing them. The main function is also in charge of the built-in commands.

## Specification
Implemented using c language along with unistd and sys libraries.

The following table shows the current set of commands supported by the shell:
| Command | Arguments | Description |
|------|----------|--------|
| cd | '', ~, /, .., - | Changes the current working directory |
| pwd | None | Writes to standard output the full path name of your current directory (from the root directory) |
| ls | ~, /,  .., -, -l, -sl, -R, -a | Displays the contents of a directory |
| mkdir | Directory name | Creates a new directory in the Linux/Unix operating system |
| rm | Directory name, -r | Deletes file or directory |
| ps | aux, eaf | views information related to the processes on a system |
| kill | -, -s, -p | Sends the specified signal to the specified processes or process groups |
| export | Expression | Marks variables and functions to be passed to child processes |
| find | -H, -L Path | locates files based on some user-specified criteria |
| echo | $Variable | Displays a line of string that is passed as the arguments or value of an environment variable |
| Process name | & | Runs the process as a background process |
## Pseudocode

The shell program should be written as the following pseudocode:

```Pseudocode
function parent_main ()
    register_child_signal (on_child_exit ())
    setup_environment ()
    shell ()


function on_child_exit ()
    reap_child_zombie ()
    write_to_log_file ("Child terminated")


function setup_environment ()
    cd (Current_Working_Directory)


function shell ()
    do
        parse_input(read_input())
        evaluate_expression ():
        switch(input_type):
            case shell_builtin:
                execute_shell_bultin ();
            case executable_or_error:
                execute_command ():

    while command_is_not_exit


function execute_shell_bultin ()
    switch(command_type):
        case cd:
        case echo:
        case export:


function execute_command ()
    child_id = fork ()
    if child:
        execvp (command parsed)
        print("Error)
        exit ()
    else if parent and foreground:
        waitpid(child)
```
## References
* https://www.geeksforgeeks.org/create-processes-with-fork-in-cpp/
* https://stackoverflow.com/questions/19099663/how-to-correctly-use-fork-exec-wait
* https://stackoverflow.com/questions/67251953/implement-cd-command-in-c
* https://stackoverflow.com/questions/17929414/how-to-use-setenv-to-export-a-variable-in-c
* http://www.microhowto.info/howto/reap_zombie_processes_using_a_sigchld_handler.html
* https://www.geeksforgeeks.org/zombie-processes-prevention/
* https://linuxhint.com/find-kill-zombie-process-linux/
* [What is a zombie process in Linux?] (https://www.tutorialspoint.com/what-is-zombie-process-in-linux)
* [Process states & dealing with zombie processes] (https://idea.popcount.org/2012-12-11-linux-process-states/)
* [Linux Signals] (https://devopedia.org/linux-signals)
* [Important notes on wait() and repeaing zombies](https://man7.org/linux/man-pages/man2/wait.2.html#NOTES)
* [sigaction(2) — Linux manual page](https://man7.org/linux/man-pages/man2/sigaction.2.html)
* [signal(2) — Linux manual page](https://man7.org/linux/man-pages/man2/signal.2.html)

## credit:
* big thanks to: https://github.com/MostafaGalal1 for letting me use his Read me
