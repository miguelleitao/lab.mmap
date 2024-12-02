#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){

    if(argc < 3){
        printf("Usage: %s src_file dest_file\n", argv[0]);
        exit(0);
    }
   
    const char *filepath = argv[1];
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0){
        printf("\n\"%s \" could not open\n",
               filepath);
        exit(1);
    }

    struct stat statbuf;
    int err = fstat(fd, &statbuf);
    if(err < 0){
        printf("\n\"%s \" could not open\n",
                       filepath);
        exit(2);
    }
    printf("size: %ld\n", statbuf.st_size);
    char *src = mmap(NULL, statbuf.st_size,
            PROT_READ,MAP_SHARED,
            fd,0);
    if(src == MAP_FAILED){
	perror("Mapping Failed");
        return 1;
    }

    // DEST
    int dfd = open(argv[2], O_RDWR|O_CREAT,0666);
    if (fd<0){
        printf("\n\"%s \" could not open\n",
               filepath);
        exit(1);
    }
    ftruncate(dfd, statbuf.st_size);

    char *dest = mmap(NULL, statbuf.st_size,
            PROT_WRITE,MAP_SHARED,
            dfd,0);
    if (dest == MAP_FAILED){
        perror("Mapping Failed");
        return 1;
    }

    memcpy(dest, src, statbuf.st_size);

    err = munmap(src, statbuf.st_size);
    if (err != 0)  printf("UnMapping src Failed\n");
    err = munmap(dest, statbuf.st_size);
    if (err != 0)  printf("UnMapping dest Failed\n");

    close(fd);
    close(dfd);

    return 0;
}

