#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int foo() {
    return 10;
}

int main() {
    struct timespec start, end;
    long long elapsedTime;
    char buffer[1024];
    int fd;
    const char* file_path = "./test_file.txt";
    const char* data = "CSC251";
    ssize_t bytes_written, bytes_read;

    // Measure getuid
    clock_gettime(CLOCK_MONOTONIC, &start);
    getuid();
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsedTime = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    printf("getuid() execution time: %lld nanoseconds\n", elapsedTime);

    // Measure getppid
    clock_gettime(CLOCK_MONOTONIC, &start);
    getppid();
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsedTime = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    printf("getppid() execution time: %lld nanoseconds\n", elapsedTime);

    // Measure getcwd
    clock_gettime(CLOCK_MONOTONIC, &start);
    getcwd(buffer, sizeof(buffer));
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsedTime = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    printf("getcwd() execution time: %lld nanoseconds\n", elapsedTime);

    // Setup and Measure write
    fd = open(file_path, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }
    clock_gettime(CLOCK_MONOTONIC, &start);
    bytes_written = write(fd, data, strlen(data));
    clock_gettime(CLOCK_MONOTONIC, &end);
    if (bytes_written == -1) {
        perror("Error writing to file");
        close(fd);
        exit(EXIT_FAILURE);
    }
    elapsedTime = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    printf("write() execution time: %lld nanoseconds\n", elapsedTime);
    close(fd);

    // Setup and Measure read
    fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }
    clock_gettime(CLOCK_MONOTONIC, &start);
    bytes_read = read(fd, buffer, sizeof(buffer));
    clock_gettime(CLOCK_MONOTONIC, &end);
    if (bytes_read == -1) {
        perror("Error reading from file");
        close(fd);
        exit(EXIT_FAILURE);
    }
    elapsedTime = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    printf("read() execution time: %lld nanoseconds\n", elapsedTime);
    close(fd);

    // Measure foo
    clock_gettime(CLOCK_MONOTONIC, &start);
    foo();
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsedTime = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    printf("foo() execution time: %lld nanoseconds\n", elapsedTime);


    return 0;
}
