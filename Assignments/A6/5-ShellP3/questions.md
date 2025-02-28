1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

My shell saves each child's process ID when it forks for piped commands and then uses waitpid() on each one. This way the shell waits for all the children to finish before showing the prompt again. If in any case i forgot to call waitpid() on all child processes, they would become zombies and might use system resources and that could crash the shell.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

After using dup2() to redirect a file descriptor, I close the original pipe ends because they aren’t needed anymore. If you leave them open it could lead to resource leaks, and the process waiting for the end-of-file (EOF) might never get it. This is because if a pipe end is still open, the system thinks there might still be more data coming, causing unexpected behaviour.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

The cd command is built-in because it changes the current working directory of the shell itself. If it were an external command it would only change the directory in a child process. Once that child ends, the shell’s directory wouldn’t be updated. Keeping it built-in makes sure that when we type cd the change affects the shell we are actually using.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

To allow any number of piped commands, I would use dynamic memory allocation instead of a fixed array which means i'd count how many commands there are first then allocate enough space for them. The trade off here is that it adds a bit of extra code complexity and I need to becareful with memory management, but it gives me the flexibility to handle any number of pipes without wasted memory.
