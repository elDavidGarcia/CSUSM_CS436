#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

static unsigned short checksum(unsigned short *ptr, int nbytes)
{

    int    sum;
    unsigned short exbyte;
    unsigned short cksum;

    sum = 0;
    while (nbytes > 1 ) {
        sum += *ptr++;
	nbytes -= 2;
    }

    if (nbytes == 1) {
        exbyte = 0;
        *((unsigned char *) &exbyte) = *(unsigned char *)ptr;
        sum += exbyte;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    cksum = ~sum;

    return cksum;

}

int main(int argc, char *argv[])
{

  unsigned short cksum, cksum2;
    unsigned char buf[512];
    int   fd;
    int   i;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename> \n", argv[0]);
        return EXIT_FAILURE;
    }

    fd = open(argv[1], O_RDONLY, 0644);
    if (fd == -1) {
        fprintf(stderr, "Error: unable to create file '%s', errno = %d (%s) \n",
                argv[1], errno, strerror(errno));
        return EXIT_FAILURE;
    }

    if (read(fd, buf, 512) != 512) {    //changed to read and brought up before checksum
      fprintf(stderr, "Error: unable to write to buffer, errno = %d (%s) \n",
	      errno, strerror(errno));
    }

    cksum = checksum((unsigned short *)&buf[0], 510);
    printf("cksum = 0x%hx \n", cksum);

    memcpy((void *)&cksum2, (void *)&buf[510], 2);
       
    if(cksum != cksum2){
      printf("ERROR");
    }

    if (close(fd) != 0) {
        fprintf(stderr, "Error: unable to close file, errno = %d (%s) \n",
 		errno, strerror(errno));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

}

