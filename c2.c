#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BLOCK_SIZE 4096 // assuming a block size of 4KB
#define SIGNATURE "\x7B\x5C\x72\x74" // the signature to search for

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <partition_device>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *partition_device = argv[1];
    int fd = open(partition_device, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buffer[BLOCK_SIZE];
    int block_number = 0;
    while (read(fd, buffer, BLOCK_SIZE) > 0) { // read blocks from the partition
        if (memmem(buffer, BLOCK_SIZE, SIGNATURE, sizeof(SIGNATURE)-1) != NULL) { // search for the signature
            printf("Signature found in block %d\n", block_number); // print the block number where the signature was found
        }
        block_number++;
    }

    close(fd);
    exit(EXIT_SUCCESS);
}

