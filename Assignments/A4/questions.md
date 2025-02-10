1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is a good choice for this application because it reads an entire line of input while limiting the number of characters read. This way it prevents buffer overflow and stops reading when it reaches a newline or EOF.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  We needed to use malloc() to allocate memory for cmd_buff in dsh_cli.c because malloc() allows us to allocate memory more dynamically at runtime which is useful when the exact size of the input isn't known at the compile time.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  build_cmd_list() must trim leading and trailing spaces from each command before storing it because if this isn't done the shell might try to execute commands with unwanted spaces in their names or arguments, and that could lead to some parsing errors by the shell.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  3 redirection exampples that we should implement in our custom shell are:
    1. Output Redirection: >
    2. Input Redirection: <
    3. Append Redirection: <<
    Issues we might have implementing these include permissions for opening and creating files & parsing the redirection symbols.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  From what i understand redirection changes the source or destinatino of i/o for a command meanwhile piping connects the output of one command directly to the input of another (like the example provided in the readme run-some-command | grep "keyword")

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  These are important to keep in a separate shell because it allows us to distinguish between normal output and error messages a lot easier which can help us keep track (logs) of our errors vs our output.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  I think our shell should handle errors by commands that fail by providing an STDERR, and if a command outputs both STDOUT and STDERR, they should both be printed but STDERR should be separated for clarity sake. The best way I could say this is you always wanna separate your good and bad apples, if you don't it could be confusing.