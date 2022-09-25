#include <stdint.h>
#include <fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>

struct instruction {
	uint8_t opcode;
	int64_t op1;
	int64_t op2;
	int64_t op3;
}__attribute__((packed));

bool IsInvalid(uint32_t ram_size, int64_t val);

bool IsInvalid(uint32_t ram_size, int64_t val){ //Check if address is valid
	if (val >= ram_size || val < 0){
	   return true;
	}
	return false;	
}

int const INITIAL_DATA=7; //sizeof(magic_word + ram_size);

int main (int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "Bad arguments\n");
	}
	
	int read_size;
	char orc[4];
	uint32_t ram_size;
	int64_t* arr;
	struct stat st;

	if (lstat(argv[1], &st) == -1) {
		err(5, "Stat failed\n");
	}
	if ((st.st_size - sizeof(orc) + 1 - sizeof(ram_size)) % 25 != 0) {
		err(6, "Bad size of file\n");
	}

	//printf("%ld\n", st.st_size);

	int fd = open(argv[1], O_RDONLY);
	if (fd == -1){
		err(2, "Could not open file\n");
	}
	
	read_size = read(fd, &orc, sizeof(orc)-1);
	if (read_size != sizeof(orc)-1) {
		err(3, "Could not read from file\n");
	}
	
	read_size = read(fd, &ram_size, sizeof(ram_size));
	if (read_size != sizeof(orc)) {
		err(3, "Could not read from file\n");
	}
	
	arr = (int64_t*)malloc(ram_size * sizeof(int64_t));
	if (arr == NULL) {
		err(4, "Memory not allocated\n");
	}
	
	struct instruction inst;
	
	while((read_size = read(fd, &inst, sizeof(inst))) > 0) {
		if (read_size == -1) {
			err(3, "Could not read from file\n");
		}			
		
		switch (inst.opcode) {
			case 0: //None
				break;
		 	
			case 149: //Set
				if (IsInvalid(ram_size, inst.op1)) {
					err(11, "Invalid address - set\n");
				}	
				arr[inst.op1] = inst.op2;
				break;

			case 93: //Load
				if (IsInvalid(ram_size, arr[inst.op2])){
					err(11, "Invalid address - load\n");
				}
				if (IsInvalid(ram_size, inst.op1)){
					err(11, "Invalid address - load\n");
				}
				arr[inst.op1] = arr[arr[inst.op2]]; 
				break;	
			
			case 99: //Store
				if (IsInvalid(ram_size, arr[inst.op1])){
					err(11, "Invalid address - store\n");
				}
				if (IsInvalid(ram_size, inst.op2)){
					err(11, "Invalid address - store\n");
				}
				arr[arr[inst.op1]] = arr[inst.op2]; 
				break;	
			
			case 145: //Jump
				if (IsInvalid(ram_size, inst.op1)){
					err(11, "Invalid address - jump\n");
				}
				if (lseek(fd, INITIAL_DATA + sizeof(inst)*arr[inst.op1], SEEK_SET) == -1) {
					err(12, "Lseek failed\n");
				}
				break;	
			
			case 37: //SGZ
				if (IsInvalid(ram_size, inst.op1)){
					err(11, "Invalid address - sgz\n");
				}
				if (arr[inst.op1] > 0) {
					if (lseek(fd, sizeof(inst), SEEK_CUR) == -1) {
						err(12, "Lseek failed\n");
					}
				}
				break;	
			
			case 173: //Add
				if (IsInvalid(ram_size, inst.op1)){
					err(11, "Invalid address - add\n");
				}
				if (IsInvalid(ram_size, inst.op2)){
					err(11, "Invalid address - add\n");
				}
				if (IsInvalid(ram_size, inst.op3)){
					err(11, "Invalid address - add\n");
				}
				arr[inst.op1] = arr[inst.op2] + arr[inst.op3];
				break;	
			
			case 51: //Mol
				if (IsInvalid(ram_size, inst.op1)){
					err(11, "Invalid address - mol\n");
				}
				if (IsInvalid(ram_size, inst.op2)){
					err(11, "Invalid address - mol\n");
				}
				if (IsInvalid(ram_size, inst.op3)){
					err(11, "Invalid address - mol\n");
				}
				arr[inst.op1] = arr[inst.op2] * arr[inst.op3];
				break;
			
			case 4: //Div
				if (IsInvalid(ram_size, inst.op1)){
					err(11, "Invalid address - div\n");
				}
				if (IsInvalid(ram_size, inst.op2)){
					err(11, "Invalid address - div\n");
				}
				if (IsInvalid(ram_size, inst.op3)){
					err(11, "Invalid address - div\n");
				}
				if (arr[inst.op3] == 0){
					err(13, "Division by zero\n");
				}
				arr[inst.op1] = arr[inst.op2] / arr[inst.op3];
				break;	
			
			case 181: //Mod
				if (IsInvalid(ram_size, inst.op1)){
					err(11, "Invalid address - mod\n");
				}
				if (IsInvalid(ram_size, inst.op2)){
					err(11, "Invalid address - mod\n");
				}
				if (IsInvalid(ram_size, inst.op3)){
					err(11, "Invalid address - mod\n");
				}
				if (arr[inst.op3] == 0){
					err(13, "Division by zero\n");
				}
				arr[inst.op1] = arr[inst.op2] % arr[inst.op3];

				break;
			
			case 193: //Out
				if (IsInvalid(ram_size, inst.op1)){
					err(11, "Invalid address - out\n");
				}
				printf("%c", (int)arr[inst.op1]);
				break;
			
			case 191: //Sleep
				if (IsInvalid(ram_size, inst.op1)){
					err(11, "Invalid address - sleep\n");
				}
				unsigned int sleepFor = arr[inst.op1]/1000;
				unsigned int result = sleep(sleepFor);
				if (result > 0) {
					err(14, "Sleep problem\n");
				}
				break;
			
			default:
				err(10, "Invalid instruction\n");
		}
	}

	exit(0);
}
