#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define BLOCK_SIZE 4096
#define MAGIC "\x7B\x5C\x72\x74"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <device> <block_number>\n", argv[0]);
        return 1;
    }

    char *device = argv[1];
    int block_num = atoi(argv[2]);

    int fd = open(device, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    off_t offset = block_num * BLOCK_SIZE;
    if (lseek(fd, offset, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    char data[BLOCK_SIZE];
    ssize_t nread = read(fd, data, BLOCK_SIZE);
    if (nread == -1) {
        perror("read");
        close(fd);
        return 1;
    }

    if (memmem(data, nread, MAGIC, strlen(MAGIC))) {
        char hex_file[20];
        sprintf(hex_file, "block%d.hex", block_num);
        char cmd[50];
        sprintf(cmd, "xxd -r -p %s %s.bin", hex_file, hex_file);
        FILE *fp = popen(cmd, "w");
        fwrite(data, nread, 1, fp);
        pclose(fp);
        printf("Block %d contains the magic signature and its bytes were saved to %s.bin\n", block_num, hex_file);
    } else {
        printf("Block %d does not contain the magic signature.\n", block_num);
    }

    close(fd);
    return 0;
}

