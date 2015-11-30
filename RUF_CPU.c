// RUF CPU v0.1 November 25, 2015

/* Goal

	RUF CPU is a program that pretends to be a CPU and memory. The idea is to have
a very simple instruction set that will allow me to experiment with translators.
It is intended strictly for learning and experimentation purposes and has no practical
application what-so-ever.

*/

/* instruction set

	0000 NOP [4-15] 0's
	0001 ADD [4-7] destination register [8-11] first register [12-15] second register
	0010 ADD [4-7] destination register [8-11] register to add to [12-15] 4 bit number
	0011 SET [4-7] destination register [8-15] 8 bit number
	0100 CPY [4-7] destination register [8-11] register to copy number from [12-15] 0's
	0101 JMP [4-7] register holding address to jump to [8-15] 0's
	0110 LD  [4-7] destination register [8-11] register holding address to load from
	0111 SV  [4-7] register with value we want to save [8-11] register holding address we want to save to
	1000 BCH [4-7] register to test [8-11] register holding address we jump to if test register is not 0 [12-15] 0's
	1001 NOT [4-7] destination register [8-11] register with value for 'not-ing'
	1010 XOR [4-7] destination register [8-11] first register [12-15] second register
	1011 OR  [4-7] destination register [8-11] first register [12-15] second register
	1100 AND [4-7] destination register [8-11] first register [12-15] second register
	1101 PRT [4-7] register holding character to print
	1110 ---
	1111 STP [4-15] 1's

*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>


int loadintomemory(unsigned short* memory, int memorylength, int ruf_file);
int execute(unsigned short* memory, int memorylength);
void printregisters(unsigned short* registers, unsigned short registercount);

int main(int argc, char** argv){

	const int MEMORY_LIMIT = 65536;
	unsigned short memory [MEMORY_LIMIT]; // 2^16
	int status = 0;

	if (argc != 2){
		fprintf(stderr, "Invalid number of arguments: %d. Must be exactly 2.\n", argc);
		return 1;
	}

	// get a file pointer on a .ruf file (should be the second argument)
	int ruf_file = open(argv[1], O_RDONLY);
	if (ruf_file == -1){
		fprintf(stderr, "The file %s was not found.\n", argv[1]);
		return 1;
	}

	// 'load' program into memory
	status = loadintomemory(memory, MEMORY_LIMIT, ruf_file);

	close(ruf_file);

	// 'execute' program
	status = execute(memory, MEMORY_LIMIT);

	return status;
}

int loadintomemory(unsigned short* memory, int memorylength, int ruf_file){

	unsigned short memorycount = 0;

	int charslength = 2;
	char buffer [charslength];

	int read_status = read(ruf_file, buffer, charslength);
	while ( read_status > 0 ){

		unsigned short instruction = buffer[0] | (buffer[1] << 8);
		//printf("%d\n", instruction);
		memory[memorycount++] = instruction;

		if (memorycount >= memorylength){
			fprintf(stderr, "Too many instructions! Memory is full.\n");
			return 1;
		}

		read_status = read(ruf_file, buffer, charslength);

	}
	printf("Read status: %d %s\n", read_status, strerror(errno));

	return 0;

}

int execute(unsigned short* memory, int memorylength){

	// 16 16-bit registers, referenced by hex-code (#0, #1, #2, ... #D, #E, #F)
	unsigned short registercount = 16;
	unsigned short registers [registercount];
	memset(registers, 0, registercount * sizeof(registers[0])); // set everything to 0
	unsigned short program_counter = 0;

	// grabs the numbers, one by one, and executes them
	int flagged = 0;
	int stopped = 0;
	unsigned short instruction = 0;
	while ( !stopped && !flagged ){

		instruction = memory[ program_counter ];

		unsigned short MNEUMONIC_MASK = 0xF000;
		unsigned short FIRST_FOUR_MASK = 0x0F00;
		unsigned short MID_FOUR_MASK = 0x00F0;
		unsigned short LAST_FOUR_MASK = 0x000F;

		unsigned short command = (instruction & MNEUMONIC_MASK) >> 12;
		unsigned short first_four = (instruction & FIRST_FOUR_MASK) >> 8;
		unsigned short mid_four = (instruction & MID_FOUR_MASK) >> 4;
		unsigned short last_four = instruction & LAST_FOUR_MASK;

		//printf("%d %d %d %d\n", command, first_four, mid_four, last_four);

		switch( command ){

			case 0:

				printf("no operation\n");

				break;
			case 1:

				printf("add two registers\n");
				registers[ first_four ] = registers[ mid_four ] + registers[ last_four ];

				break;
			case 2:

				printf("add a number to one register\n");
				registers[ first_four ] = registers[ mid_four ] + last_four;

				break;
			case 3:

				printf("set\n");
				registers[ first_four ] = (mid_four << 16) | last_four;

				break;
			case 4:

				printf("copy\n");
				registers[ first_four ] = registers[ mid_four ];

				break;
			case 5:

				printf("jump\n");
				program_counter = registers[ (first_four << 16) | mid_four ];

				break;
			case 6:

				printf("load\n");
				registers[ first_four ] = memory[ registers[ mid_four ] ];

				break;
			case 7:

				printf("save\n");
				memory[ registers[ mid_four ] ] = registers[ first_four ];

				break;
			case 8:

				printf("branch\n");
				if ( registers[ first_four ] != 0 ){
					program_counter = registers[ mid_four ];
				}

				break;
			case 9:

				printf("not\n");
				registers[ first_four ] = ~registers[ mid_four ];

				break;
			case 10:

				printf("exclusive or\n");
				registers[ first_four ] = registers[ mid_four ] ^ registers[ last_four ];

				break;
			case 11:

				printf("or\n");
				registers[ first_four ] = registers[ mid_four ] | registers[ last_four ];

				break;
			case 12:

				printf("and\n");
				registers[ first_four ] = registers[ mid_four ] & registers[ last_four ];

				break;
			case 13:

				printf("print\n");
				printf("%c", registers[ first_four ]);

				break;
			case 15:

				printf("stop\n");
				stopped = 1;

				break;
			default:

				fprintf(stderr, "Invalid command.\n");
				return 1;

				break;

		}

		printregisters(registers, registercount);

		program_counter++;

	}

	return 0;

}

void printregisters(unsigned short* registers, unsigned short registercount){

	printf("r0:%d\tr1:%d\tr2:%d\tr3:%d\t\n"
			"r4:%d\tr5:%d\tr6:%d\tr7:%d\t\n"
			"r8:%d\tr9:%d\tr10:%d\tr11:%d\t\n"
			"r12:%d\tr13:%d\tr14:%d\tr15:%d\t\n",
			registers[0], registers[1], registers[2], registers[3],
			registers[4], registers[5], registers[6], registers[7],
			registers[8], registers[9], registers[10], registers[11],
			registers[12], registers[13], registers[14], registers[15]);

}
