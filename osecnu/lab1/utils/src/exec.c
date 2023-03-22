#include "../include/exec.h"

/**
 * @note helper functions
*/
static void copy_argv(char* src[], char dest[][kMaxTokenLen], int cnt);
static char **deep_copy(char argv[][kMaxTokenLen], int argc);
static inline void run_node(cnode* head, int n);
static inline void run_redir_node(cnode* list, int n);

int pgetline(char* buf, int len) {
    printf("sh> ");   
    memset(buf, 0, len);
    fgets(buf, len, stdin);
#ifdef SHDEBUG
    printf("successfully parsed cmdline: %s\n", buf);
#endif

    // Write parsed cmdline into history file
    FILE* shfp = fopen(HISTORY_PATH, "a");
    if (shfp) {
        fprintf(shfp, "%s", buf);
        fclose(shfp);
    } else {
        fprintf(stderr, "Open history file error!\n");
        return 1;
    }
    
    if (buf[0] == 0) {
        return 0;
    }

    return 1;
}

cnode* parsecmd(char* buf) {
    cnode* header = init();
    int cnt = 0;
    char* argv[kMaxTokenLen];

    char* next_token = strtok(buf, " ");
    while (next_token != NULL) {
        if (!strcmp(next_token, "|")) {
            // left of pipe cannot be zero tokens
            if (cnt == 0) {
                fprintf(stderr, "ShError: left of pipe cannot be zero tokens.\n");
                header->cmdline.type = WrongMode;
                return header;
            }
            pcmd temp = {
                .argc = cnt,
                .type = Exec,
            };
            copy_argv(argv, temp.argv, cnt);
            insert(header, temp);

            cnt = 0;
        } else if (!strcmp(next_token, "&\n")) {
            // Like zsh, Background mode can not be used under pipeline and redir
            // Also, & must be set at the end of a cmdline, 
            // which means token next to "&" starts a new cmdline
            // So we have to check this here
#ifdef SHDEBUG
            printf("SHDEBUG: this token is parsed as background\n");
#endif
            header->cmdline.type = Background;
            break;
        } else if (!strcmp(next_token, ">")
                || !strcmp(next_token, ">>")
                || !strcmp(next_token, "<")) {
            // go to the redir mode
            // next token indicates which file should be redir to
            // after that, tokens represents a new cmdline
            // I manage redir mode into this: 
            // In dummy head, argv[0] saves the redir op("<", ">", ">>")
            // argv[1] saves the redir file
            header->cmdline.type = Redir;
            strcpy(header->cmdline.argv[0], next_token);

            // After that, we need to terminate the input cmdline and return
            next_token = strtok(NULL, " ");

            if (!next_token) {
                fprintf(stderr, "ShError: One token should be after redir op.\n");
                header->cmdline.type = WrongMode;
                return header;
            }
            next_token[strlen(next_token) - 1] = 0; // extract the "\n"
            strcpy(header->cmdline.argv[1], next_token);

            // Package the cmd block and insert into linked list.
            pcmd temp = {
                .argc = cnt,
                .type = Exec,
            };
            copy_argv(argv, temp.argv, cnt);
            insert(header, temp);
            return header;
        } else {
            argv[cnt] = (char*)malloc(kMaxTokenLen);
            strcpy(argv[cnt++], next_token);
        }
        next_token = strtok(NULL, " ");
    }

    // shift the last token "\n" to "\0"
    if (header->cmdline.type != Background) {
        argv[cnt - 1][strlen(argv[cnt - 1]) - 1] = 0;
    }

    pcmd temp = {
        .argc = cnt,
        .type = Exec,
    };
    copy_argv(argv, temp.argv, cnt);
    insert(header, temp);

    return header;
}

void eval(char* cmdline) {
    // parsecmd into argc, argv and CmdType
    cnode* list = parsecmd(cmdline);

    // Exception handler
    if (list->cmdline.type == WrongMode) {
        fprintf(stderr, "ShError: Wrong Input Mode.\n");
        return;
    }
#ifdef SHDEBUG
    print_list(list);
    printf("eval: number of nodes: %d\n", list->cmdline.argc);
#endif

    if (list->cmdline.argc == 1) { // No pipeline
        if (list->cmdline.type == Background) {
#ifdef SHDEBUG
            printf("SHDEBUG: Background: Running in background\n");
#endif  
            signal(SIGCHLD, SIG_IGN);
            pid_t pid;
            if ((pid = fork()) == 0) {
                int fd = open("/dev/null", O_RDONLY);
                dup2(fd, STDIN_FILENO);
                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
                
                close(fd); 

                run_node(list, 1);
                exit(1);
            } else {
                printf("Child process pid=%d is running at background\n", pid);
            }
        } else if (list->cmdline.type == Redir) {
            run_redir_node(list, 1);
        } else {
            pid_t pid;
            if ((pid = fork()) == 0) {
                run_node(list, 1);
            } else {
                wait(0);
            }
        }
    } else {
        run_pipeline(list, list->cmdline.argc);
    }
    delete_list(list);
}

void runcmd(int argc, char* argv[]) {
    assert(argc);
    int builtin_op;
#ifdef SHDEBUG
    for (int i = 0; i < argc; i++) {
        printf("runcmd: argv[%d]: %s\n", i, argv[i]);
    }
    printf("runcmd: stdin: %d, stdout: %d\n", fileno(stdin), fileno(stdout));
    // char* arg[] = {"echo", "a", NULL};
#endif
    if ((builtin_op = builtin_command(argv[0]))) {
        bexec(builtin_op, argv);
        exit(0);
    } else if (execvp(argv[0], argv) < 0) {
        fprintf(stderr, "ShError: Execvp error!\n");
        exit(1);
    }
    exit(0);
}

void run_pipeline(cnode* head, int number) {
    int fd[number][2];

    for (int i = 0; i < number; i++) {
        pipe(fd[i]);
        pid_t pid = fork();
        if (pid == 0) {
            if (i == 0) { // The first program
                dup2(fd[i][1], STDOUT_FILENO);
                run_node(head, i + 1);
            } else if (i == number - 1) { // The last program
                dup2(fd[i - 1][0], STDIN_FILENO);
                run_node(head, i + 1);
            } else {
                dup2(fd[i - 1][0], STDIN_FILENO);
                dup2(fd[i][1], STDOUT_FILENO);
                run_node(head, i + 1);
            }
            exit(0);
        }
        close(fd[i][1]);
    }
    for (int i = 0; i < number; i++) {
        close(fd[i][0]);
    }
    for (int i = 0; i < number; i++) {
        wait(NULL);
    }
}

void pchdir(char* buf) {
    buf[strlen(buf)-1] = 0; // switch '\n' to '\0' inorder to handle arg[1] normally
#ifdef SHDEBUG
    printf("Ready to chdir to %s\n", buf+3);
#endif
    if (chdir(buf+3) < 0) {
        fprintf(stderr, "Sherror: invalid argument: %s\n", buf+3);
    }
}


static void copy_argv(char* src[], char dest[][kMaxTokenLen], int cnt) {
    for (int i = 0; i < cnt; i++) {
        strcpy(dest[i], src[i]);
    }
    // src[cnt] = NULL;
}

static char** deep_copy(char argv[][kMaxTokenLen], int argc) {
    char** new_argv = malloc((argc + 1) * kMaxTokenLen);
    for (int i = 0; i < argc; i++) {
        new_argv[i] = malloc(kMaxTokenLen);
        strcpy(new_argv[i], argv[i]);
    }
    new_argv[argc] = NULL;
    return new_argv;
}

static inline void run_node(cnode* head, int n) {
    int argc = get(head, n).argc;
    char** argv = deep_copy(get(head, n).argv, argc);

    runcmd(argc, argv);
    exit(1);
}

static inline void run_redir_node(cnode* list, int n) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        if (!strcmp(list->cmdline.argv[0], "<")) {
            int fd = open(list->cmdline.argv[1], O_RDONLY);
            if (fd < 0) {
                fprintf(stderr, "Redir '<': open error\n");
                exit(EXIT_FAILURE);
            }
            if (dup2(fd, STDIN_FILENO) < 0) {
                fprintf(stderr, "Redir '<': dup2 error\n");
                exit(EXIT_FAILURE);
            }
        } else if (!strcmp(list->cmdline.argv[0], ">")) {
            int fd = open(list->cmdline.argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fd < 0) {
                fprintf(stderr, "Redir '>': open error\n");
                exit(EXIT_FAILURE);
            }
            if (dup2(fd, STDOUT_FILENO) < 0) {
                fprintf(stderr, "Redir '>': dup2 error\n");
                exit(EXIT_FAILURE);
            }
        } else if (!strcmp(list->cmdline.argv[0], ">>")) {
            int fd = open(list->cmdline.argv[1], O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (fd < 0) {
                fprintf(stderr, "Redir '>>': open error\n");
                exit(EXIT_FAILURE);
            }
            if (dup2(fd, STDOUT_FILENO) < 0) {
                fprintf(stderr, "Redir '>>': dup2 error\n");
                exit(EXIT_FAILURE);
            }
        } else {
            fprintf(stderr, "ShError: eval goes to Redir with no operator!\n");
            exit(1);
        }
        // We have done the redir progress before, then we execute the function
        run_node(list, n);
        exit(1);
    } else {
        wait(0);
    }
}