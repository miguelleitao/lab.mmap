#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    if(argc < 2){
        printf("File path not mentioned\n");
        exit(0);
    }
   
    const char *filepath = argv[1];
    int fd = open(filepath, O_RDONLY);
    if(fd < 0){
        perror("Open file");
        exit(1);
    }

    struct stat statbuf;
    int err = fstat(fd, &statbuf);
    if(err < 0){
        printf("\n\"%s \" could not open\n",
                       filepath);
        exit(2);
    }
    printf("File size: %ld\n", statbuf.st_size);

    char *ptr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED){
	perror("Mapping Failed");
        printf("Mapping Failed\n");
        return 1;
    }
    close(fd);

    int i;
    for( i=0 ; i<statbuf.st_size ; i++ )
	putchar(ptr[i]);
   
    err = munmap(ptr, statbuf.st_size);
    if (err != 0){
        printf("UnMapping Failed\n");
        return 1;
    }

    return 0;
}

