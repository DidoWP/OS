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

int main (int argc, char* argv[]) {

    if ( argc != 3 ) {
		err(1, "Bad arguments\n");
    }

    if ( strcmp(argv[1], "-d") == 0 ){
		char c;

		while ( read(0 , &c, sizeof(c)) > 0 ) {
			bool flag = false;
			for ( size_t i = 0; i < strlen(argv[2]); i++) {
				if( argv[2][i] == c ) {
					flag = true;
				}	
			}
			if( flag == false ) {
				printf("%c", c);
			}			
		}
	}
	else if ( strcmp(argv[1], "-s") == 0 ) {
		char curr;
		char last = '\0';

		while ( read(0 , &curr, sizeof(curr)) > 0 ) {
			bool flag = false;
			if ( curr == last ) {
				for ( size_t i = 0; i < strlen(argv[2]); i++) {
					if( argv[2][i] == curr ) {
						flag = true;
					}	
				}
			}
			if( flag == false ) {
				printf("%c", curr);
				last = curr;
			}			
		}
	}
	else {
		if( strlen(argv[1]) != strlen(argv[2]) ) {
			err(2, "String have different lenghts\n");
		}

		char c;
		while ( read(0 , &c, sizeof(c)) > 0 ) {
			bool flag = false;
			for ( size_t i = 0; i < strlen(argv[1]); i++) {
				if( argv[1][i] == c ) {
					flag = true;
					printf("%c", argv[2][i]);
				}	
			}
			if( flag == false ) {
				printf("%c", c);
			}			
		}
	}

    exit(0);
}

