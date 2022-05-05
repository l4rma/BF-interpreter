#include <stdio.h>
#include <stdlib.h>

void executeInstruction(char** memPtr, char* inst, int* i);

int main(int argc, char* argv[]) {
	// Make sure there's an argument
	if (argc != 2) {
		fprintf(stderr, "Usage: ./bf <file>\n");
		return 1;
	}
	char* fileName;		// File with instructions
	char* memPtr;		// Points to what instruction we are reading from the inputfile
	int instIdx;		// Points to memory 
	char* memory;		// Store bytes before printing
	char* instructions;	// Store the instructions written from the file
	FILE *f;			// File to be loaded as an argument
	int i;				// For loops
	
	// Setup
	fileName = argv[1];
	memory = malloc(sizeof(char)*0x1000); 
	memPtr = memory;
	instIdx = 0;

	// Open file and read
	f = fopen(fileName, "r");
	if (!f) {
		fprintf(stderr, "Error: Could not open file %s\n", fileName);
		return 1;
	}
	fseek(f,0,SEEK_END);
	int len = ftell(f);
	instructions = malloc(sizeof(char)*len);
	rewind(f);
	for (i = 0; i < len; i++) {
		instructions[i] = fgetc(f);
	}
	
	// Execute instructions
	while(instIdx < len - 1) {
		executeInstruction(&memPtr, instructions, &instIdx);
		instIdx++;
	}

	fclose(f);
	free(memory);
	free(instructions);

	return 0;
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
