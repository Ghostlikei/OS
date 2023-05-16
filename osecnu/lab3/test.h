/**
 * @name test.h
 * @author Tongle Shen (10215501403@stu.ecnu.edu.cn)
 * @date 2023/5/16
 * @brief Test RAM/ROM i/o speed on minix system
*/
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <assert.h>
#include <aio.h>

#define TEST void

/**
 * @brief params of the whole test
*/
#define REPEAT           100    // repeat times of read/write
#define FILE_INIT_SIZE   256   // Init size(MB) of testing files

const int kTestBlockSizes[6] = {64, 256, 1 << 10, 4 << 10, 16 << 10, 64 << 10}; 

const char* kTestFileName[] = {"./a.txt"}; // Not clear yet

// Utils for testing

/**
 * @param file_size: size of new file in "MB"
 * @param filepath: path of the new file you would like
*/
void init_file(int file_size, const char* filepath);

void write_file(int block_size, int is_rand, const char* w_file_path, int repeat);

void read_file(int block_size, int is_rand, const char* r_file_path, int repeat);

long get_time_left(long start_time, long end_time);

/**
 * @brief testing functions
*/
TEST throughput_test();
TEST blocksize_test();