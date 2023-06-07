# OSN Assignment-3 README File

## **Additions for Assignment-3 since the original shell from Assignment-2**
---
* Since no relation between & and piping was given in the assignmnet, they have not been handled in any definite manner (It still works)

* Arrow keys are not registered and not printed on the terminal as they are not required for the implementation.
* Some functionalites of Ctrl + Z are a bit weirdly implemented in bash itself and they are kept the same here. For eg. `Sleep 10` followed by `Ctrl + Z` and then `fg 1 after 10 seconds` might not run sleep again and straightaway exits. (`Works the same way in bash`)
* Only the asked tab functionalites are implemented. Functions like `/` and then Tab won't print the content of the / directory as it is not mentioned in the assignment.
---
## **Assumptions** :

* Assuming discover won't be run on folders like `/` or `/home` since it gives segfault because of the large directory size or some permission to open some files in the snap folder. Running it on any folder after `/home/{username}/<folder_name>` for eg. Desktop works fine.
* Assuming flags like `-laaaaaaa` or anything other than the 4 mentioned in pdf won't be passed to ls since we weren't asked to implement those.
* Taking only one target_directory as input in discover.
* Exit function kills all background processed before exiting.
* ls uses `strcasecmp` to sort as alphabetically sort was mentioned.
* Unexpected token error handled with `;` but not with `&`. Throws an error when entered commands like `cd .. ;;` because of 2 semi-colons but just runs as normal background process in case of double `&` like `sleep 30 & &`.
* When wrong command is entered as background process, mimics shell behaviour and output format as well where I create a process and then later exit from it with exit status 108.
* Flags like `-dfff` and likes are not handled in discover.
* **Colour coding**
  * Red for directory
  * Green for executables (any of the x permission is on)
  * White for files otherwise
  * Errors printed in yellow.

---
## Directions
1. Run `make` or preferably, `make all` which removes the previous a.out and generates a new executable a.out.
2. Start the shell with `./a.out` which opens the shell with home directory set as the current directory.
3. `exit` function is there to exit the program.
---
The following sections describes what different parts of my code does in brief.
## Division of code within files
---
### **main.c**
--- 
Serves as the driver code for the shell, it initializes most of the global variables and arrays. It locates history text files by accessing `/proc/self/exe` and then opens them using `fopen`. It contains a loop part after this setup part which runs with while loop(1) until `exit` commnd is entered. The loop part does the following :
* It first checks if any background process that was running has ended and prints that if there are any while also removing it from the array of pid_lists.
* Asks for input
* Processes the input to understand which command to run in background and which in foreground.
* Calls functions from other .c files to run based on what input is given.
* It also handles keeping track of which background processes are currently running.
* When `exit` is entered, it kills all child processes running before exiting itself.

---
### **cd.c**
---
Gets the input, and changes to the directory based on that. If the directory is not found it prints the corresponding error and also does the same if there are too many arguments. It maintains an old_wd variable for keeping track of the last directory which is used in `cd -` implementation. Returns error codes to main.c at the end.

---
### **discovery.c**
---
Checks which flags are turned on and whether there is a target file the user is looking for. Calls function bsaed on whether there is a target file and keeps calling that function recursively while printing the file/directory name to stdout.

---
### **echo_this.c**
---
Takes the input and prints that to stdout. Prints a new line if passed with arguments and takes care of spaces and tabs.

---
### **getpinfo.c**
---
When `pinfo` is passed without an argument, it prints the process information of the current running process, otherwise it prints the process information of the process whose pid is entered. Procures information from `/proc/[pid]/stat` and the path from `/proc/[pid]/exe`. It checks if the process is background or foreground by comparing the Groups PID of the current foreground process and the process we are looking for.

---
### **headers.h**
---
Contains all the headers and the defined constants. This is the header file we include in all the .c files.

---
### **hist.c**
---
Prints the history, basically the last 10 commands (if there are at least 10), otherwise however many there are present. Note that according to my shell (and `bash` as well), `echo` and `echo ` are two different commands (the second one has a ' ' at the end).

---
### **list_em.c**
---
Finds the flags that are enabled and then lists out all the directories and files in the required format. Note that alignment is not handled when `-l flag` is off as it was not mentioned. However the text is right-aligned when it is on.

---
### **othercmd.c**
---
Any command that is not built in by me goes on to this with a flag which tells it if it's a bacground process or a foreground process. Waits for the child if foreground otherwise it continues. Returns the process ID of the child process and the child process terminates with `error code 108` if `execvp` fails (command not found etc). Uses `gettimeofday` to calculate the time taken.

---
### **print_wd.c**
---
Implements pwd and prints the current working directory.

---
### **prompt.c**
Prints the prompt every time the loop happens, it checks whether the home directory is an ancestor of the current working directory and prints ~ or doesn't do it accordingly using `strncmp`. It also checks if condtions match for time to be printed to the terminal of the last foreground process executed by `execvp`.

---
### **input.c**
It takes the input in raw mode and processes it, also implementing the Tab auto completion functionality. The code given to us was used and edited for implementing this.

---
### **jobs.c**
Jobs command handles and prints all the background processes in alphabetical order as asked. It opens the `/proc` folder and accesses all the processes one by one. Two flags -r and -s are also implemented.

---
### **foreg_g.c**
Sends a process to the foreground and changes it state to running. Achieves this by sending it the `SIGCONT` signal and hands it the terminal via `tcsetpgrp`. The terminal is taken back by the same command after ignorign `SIGTTOU`. It uses `WUNTRACED` with waitpid for the implementation of Ctrl + Z.

---
### **back_g.c**
Sends `SIGCONT` signal to a process in the background to change it state to running.

---
