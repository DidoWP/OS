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

// бих искал да видя по елегантно решение

int main (int argc, char* argv[]) {

	char buff;

	if ( argc == 1 ) {
		while( read(0, &buff, sizeof(buff) > 0 )) {
			fprintf(stdout, "%c", buff);
		}
	}
	
	if ( strcmp(argv[1], "-n") != 0 )
	{
		for(int i = 1; i <= argc-1; i++) {
			char buff2;
			int fd1 = open(argv[i], O_RDONLY);
			if (fd1 == -1) {
				err(2, "Couldn'n open file\n");	
			}
			
			while( read(fd1, &buff2, sizeof(buff2) > 0) ) {
				printf("%c", buff2);
			}	
		}
	}
	else {
		for(int j = 2; j <= argc-1; j++) {
			char buff3;
			int br = 1;
			int fd2 = open(argv[j], O_RDONLY);
			if ( fd2 == -1 ) {
				err(3, "Could not open file\n");
			}

		//	if (setvbuf(stdin, buff3, _IOLBF, sizeof(buff3)) != 0){
		//		printf("Incorrect type or size of buffer\n");
		//	}
			printf("%d%c", br, ' ');
			while( read(fd2, &buff3, sizeof(buff3) > 0)) {
				fprintf(stdout, "%c", buff3);
				
				if(buff3 == '\n'){
					br++;
					printf("%d%c", br, ' ');
				}
			}
		}
	}

	exit(0);
}
