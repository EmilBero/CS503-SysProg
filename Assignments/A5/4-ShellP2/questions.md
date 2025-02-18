1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  Using 'fork' before we use 'execvp' allows for the shell to continously run whilst the command runs. This is because fork creates a new child process allowing the parent (shell), continues to run.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  if the fork() system call fails, it returns an error message and the shell continues running instead of crashing, this way if 1 command is causing the fork system call not to work for whatever reason, it won't stop us from using others.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  From what i understand, execvp() finds the command to eexecute via the PATH env variable, where binaries are stored. 

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The purpose of calling wait() in the parent process is to make sure that the parent process doesn't proceed until the child process finishes executing. If you were to NOT call wait(), you could get zombie processes which could affect the programs performances.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() is important because it extracts the exit status of a child process that's been terminated which helps us know wether the command ran into an error or not. Overall good for error handling and/or debugging.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  My implementation of build_cmd_buff() handles quoted arguments by treating anything after a double quotes (") as a single argument until a second (") double quote is encountered. This is necessary for handling commands with quoted arguments to pass multiple-words, filesnames or watever the case could be.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  The main parsing logic differences between this assignment and the last was of course modifying the quoting arguments. This might not be exactly what the question is asking but one unexpected challenge was not having access to my work station (main computer), as i am currently traveling I had to set up a new system with WSL and i ran into a lot of issues trying to setup WSL on this, however once I got WSL up and running and linked a new SSH key to my github and cloned the old code. Overall wasn't hard just very tedious.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals in a linux system are a form of synchronous interprocess communication (IPC) used to notify processes of events (terminations, interrupts, exceptions). Signals are different from other IPCs because they are lightweight and don't required shared memory ora persistent communication channel. They are instead, simple notifications sent by the kernel or other processes to help alter behaviors.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**: 
    SIGKILL (Signal 9) - forcefully terminates a process and cannot be caught, ignored, or handled. It is typically used when a process is unresponsive or must be immediately stopped.
    SIGTERM (Signal 15) - a graceful termination signal that allows a process to clean up resources before exiting. It can be caught and handled by the process to perform necessary shutdown procedures.
    SIGINIT (Signal 2) - sent when a user presses Ctrl + C in the terminal, typically used to interrupt a running process. It can be caught by the process to handle cleanup before termination.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  When a process receives SIGSTOP it is immediately paused by the operating system, unlike the others this one can't be ignored. This is because SIGSTOP is designed to be an unconditional signal that allows the system or user to suspend the processes very reliably.
