#include <stdio.h>
#include <stdlib.h>

void executeInstruction(char** memPtr, char* inst, int* i);
char* readInstructions(FILE* f, int len);
int getFileLength(FILE* f);

int main(int argc, char* argv[]) {
	// Exit program if no argument
	if (argc != 2) {
		fprintf(stderr, "Usage: ./bf <file>\n");
		return 1;
	}
	char* fileName;		// File with instructions
	char* memPtr;		// Points to memory
	int instIdx;		// Instruction index
	char* memory;		// Store bytes interpreted from the instructions
	int i;				// For loops index
	
	// Setup
	fileName = argv[1];
	memory = malloc(sizeof(char)*0x1000); 
	memPtr = memory;
	instIdx = 0;

	FILE *f;			// File to be loaded as an argument

	f = fopen(fileName, "r");
	if (!f) {
		fprintf(stderr, "Error: Could not open file %s\n", fileName);
		return 0;
	}

	int len = getFileLength(f);

	// Open file and read
	char* instructions = readInstructions(f, len);
	
	// Execute instructions
	while(instIdx < len - 1) {
		executeInstruction(&memPtr, instructions, &instIdx);
		instIdx++;
	}

	free(memory);
	free(instructions);

	return 0;
}

int getFileLength(FILE* f) {
	fseek(f,0,SEEK_END);
	int len = ftell(f);
	rewind(f);
	return len;
}

char* readInstructions(FILE* f, int len) {
	char* instructions;	// Store the instructions written from the file
	instructions = malloc(sizeof(char)*len);
	for (int i = 0; i < len; i++) {
		instructions[i] = fgetc(f);
	}
	fclose(f);
	return instructions;
}

void executeInstruction(char** memPtr, char* inst, int* i) {
	char op = inst[*i];
	int b = 0;
	switch(op) {
		case '>': *memPtr += 1; break;
		case '<': *memPtr -= 1; break;
		case '+': **memPtr += 1; break;
		case '-': **memPtr -= 1; break;
		case '.': putchar(**memPtr); break;
		//TODO: Implement instruction
		case ',': break;
		case '[':
			if (!**memPtr) { // If the data at the pointer is zero, jump to ']'
				b = 1;
				while(b) {
					*i += 1;
					if(inst[*i] == '[') {
						b++;
					} else if(inst[*i] == ']') {
						b--;
					}
				}
			}
			break;
		case ']':
			if (**memPtr) { // If the data at the pointer is not zero, jump to '['
				b = 1;
				while(b) {
					*i -= 1;
					if(inst[*i] == ']') {
						b++;
					} else if(inst[*i] == '[') {
						b--;
					}
				}
			}
			break;
		// default:
		// 	fprintf(stderr, "Unknown symbol: '%c'\n", op);
	}
}
