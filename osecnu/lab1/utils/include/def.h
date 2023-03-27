/**
 * @author Tongle Shen(10215501403@stu.ecnu.edu.cn)
 * @name def.h
 * @date 2023.3.16
 * @brief some constants, macros, and enums
*/
#pragma once

#define kMaxArgs 20
#define kMaxTokenLen 64
#define kMaxTokens 1024
#define kMaxHistory 256

/**
 * @note macros for mytop
*/
#define kMaxProcess 512
#define kMaxProcessTop 32

#ifdef  __aarch64__
    #define HISTORY_PATH "/Users/shenqingyun/Desktop/git_repository/OS/osecnu/lab1/.sh_history"
#else
    #define HISTORY_PATH "/root/git_repo/OS/osecnu/lab1/.sh_history"
#endif

/**
 * @note enum cmd type
*/
typedef enum CmdType {
    Exec,
    Background,
    Redir,
    Pipe,
    WrongMode,
} CmdType;

typedef enum ShError {
    Normal,
    ChdirError,
    ForkError,
    ExecError,
    InvalidArgs,
    IOError,
    ModeError,
} ShError;