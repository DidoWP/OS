#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
	
	if (argc != 3) {
		errx(1, "Enter 2 arguments\n");
	}

	uint8_t bits[8] = {1,2,4,8,16,32,64,128};
	uint16_t resultOne[8] = {2,8,32,128,512,2048,8192,32768};
//	uint16_t resultZero[8] = {1,4,16,64,256,1024,4096,16384};

	int fd1, fd2;
	fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1) {
		err(2, "Could not open file\n");
	}
	fd2 = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd2 == -1) {
		err(3, "Could not create file\n");
	}

	uint16_t curr;
	while (read(fd1, &curr, sizeof(curr)) > 0) {
		uint8_t result = 0;
		
		/*int read_size = read(fd1, &curr, sizeof(curr));
		if (read_size != sizeof(curr)) {
			err(4, "Could not read from file\n");
		}*/

		for(int i=0; i<8; i++) {
			if ((curr & resultOne[i]) > 0) {
				result += bits[i];
			}
		}

		write(fd2, &result, sizeof(result));
	}

	exit(0);
}
