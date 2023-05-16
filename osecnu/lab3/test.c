/**
 * @name test.c
 * @author Tongle Shen (10215501403@stu.ecnu.edu.cn)
 * @date 2023/5/16
 * @brief Test RAM/ROM i/o speed on minix system
*/
#include "test.h"

void init_file(int file_size, const char* filepath) {
    size_t sizeInMB = file_size;
    int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return;
    }

    const size_t bufferSize = 1024; // Buffer size for writing data
    char buffer[bufferSize];
    ssize_t bytesToWrite = sizeInMB * 1024 * 1024; // Convert size from MB to bytes

    printf("Init file (%s) : writing with junks.\n", filepath);
    // Fill buffer with junk characters
    for (size_t i = 0; i < bufferSize; i++) {
        buffer[i] = 'A' + (i % 26); // Fill with letters A-Z in a cyclic pattern
    }

    // Write junk data to the file
    while (bytesToWrite > 0) {
        ssize_t bytesWritten = 
            write(fd, buffer, (bytesToWrite < (ssize_t)bufferSize) ? bytesToWrite : bufferSize);
        if (bytesWritten == -1) {
            perror("write");
            close(fd);
            return;
        }

        bytesToWrite -= bytesWritten;
    }
    printf("Finish init file (%s).\n", filepath);

    close(fd);
}

void write_file(int block_size, 
                int is_rand, 
                const char* w_file_path,
                int repeat) {
    // Open the file with proper flags
    int flags = O_WRONLY | O_SYNC;
    int fd = open(w_file_path, flags, 0644);
    if (fd == -1) {
        perror("open");
        return;
    }

    // Get the infomation from the file
    lseek(fd, 0, SEEK_END);
    off_t file_size = lseek(fd, 0, SEEK_END);
    off_t max_offset = file_size - block_size;

    char* buffer = (char* )malloc(block_size * sizeof(char));
    if (buffer == NULL) {
        perror("malloc");
        close(fd);
        return;
    }
    memset(buffer, 7, block_size); // Fill the temp buffer with junks
    
    // printf("Write file (%s): is_random(%d), blocksize(%d)\n", w_file_path, is_rand, block_size);

    for (int i = 0; i < repeat; i++) {
        // Prepare offsets
        off_t offset = is_rand ? random() % max_offset : 0;

        ssize_t bytesWritten = 0;
        ssize_t remainingBytes = block_size;

        // Set the pointer to a certain position according to is_rand
        if (lseek(fd, offset, is_rand ? SEEK_SET : SEEK_CUR) == -1) {
            perror("lseek");
            free(buffer);
            close(fd);
            return;
        }

        /**
         * Write buffer into the file with block size
         * If write() touched the end of the file, it will return to the beginning
         * of the file and write the remaining bytes
        */
        while (remainingBytes > 0) {
            ssize_t written = write(fd, buffer, remainingBytes);
            if (written == -1) {
                perror("write");
                free(buffer);
                close(fd);
                return;
            }

            bytesWritten += written;
            remainingBytes -= written;

            if (bytesWritten < block_size) {
                lseek(fd, 0, SEEK_SET);
            }
        }

        // After writing, byte written is supposed be the same as block_size
        assert(bytesWritten == block_size);
    }
    

    free(buffer);
    close(fd);
    // printf("Write file (%s): is_random(%d), blocksize(%d) ENDED\n", w_file_path, is_rand, block_size);
}

int main() {
    init_file(FILE_INIT_SIZE, kTestFileName[0]);

    clock_t t1 = clock();
    write_file(kTestBlockSizes[5], 1, kTestFileName[0], REPEAT);
    clock_t t2 = clock();
    write_file(kTestBlockSizes[5], 0, kTestFileName[0], REPEAT);
    clock_t t3 = clock();

    double executionTime1 = (double) (t2 - t1) / CLOCKS_PER_SEC;
    double executionTime2 = (double) (t3 - t2) / CLOCKS_PER_SEC;

    printf("Rand write time: %lfs\n", executionTime1);
    printf("Seq write time: %lfs\n", executionTime2);
}