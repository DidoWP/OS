#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>

struct res {
    uint16_t off;
    uint8_t b1;
	uint8_t b2;
}__attribute__((packed));

int main(int argc, char* argv[]) {
        int fd1;
        int fd2;
		int fd3;
		struct stat st1;
		struct stat st2;
		uint8_t rdfd1;
		uint8_t rdfd2;

		if ( argc != 3 ){
			err(1, "Enter 2 arguments!");
		}

        if ( (fd1 = (open(argv[1], O_RDONLY))) == -1 ) {
                fprintf(stderr, "Open fd1 failed!\n");
                exit(1);
        }
        if ( (fd2 = ( open(argv[2], O_RDONLY))) == -1 ) {
                fprintf(stderr, "Open fd2 failed!\n");
                exit(1);
        }

		fstat(fd1, &st1);
		fstat(fd2, &st2);
		
		if ( st1.st_size != st2.st_size ) {
			fprintf(stderr, "Files have different size!\n");
			exit(2);
		}
		if ( st1.st_size > 65536 ) {
			warnx(1, "The offcet may not be enough.\n");
		}

        fd3 = open("patch.bin", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
		if (fd3 == -1) {
			err(2,"Filed to create file");
		}
	
		uint16_t br = 0;	
		while( read(fd1, &rdfd1, sizeof(rdfd1) > 0) ) { 
		
			read(fd2, &rdfd2, sizeof(rdfd2));
		
			if ( rdfd1 != rdfd2 ) {
				struct res diff;
				diff.off = br;
				diff.b1 = rdfd1;
				diff.b2 = rdfd2;
				write(fd3, &diff, sizeof(diff));
			}
			br++;
		}
    
        close(fd1);
        close(fd2);
		close(fd3);
        exit(0);
}
