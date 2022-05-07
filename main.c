#include <stdio.h>
#include <stdlib.h>

void executeInstruction(char** memPtr, char* inst, int* i);

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: ./bf <file>\n");
		return 1;
	}

	FILE *f;
	int instIdx, fileLength;
	char *memory, *memPtr, *instructions, *fileName;
	
	memory = malloc(sizeof(char)*0x1000); 
	memPtr = memory;
	fileName = argv[1];
	f = fopen(fileName, "r");
	if (!f) {
		fprintf(stderr, "Error: Could not open file %s\n", fileName);
		return 0;
	}
	fseek(f,0,SEEK_END);
	fileLength = ftell(f);
	rewind(f);

	instructions = malloc(sizeof(char)*fileLength);
	for (instIdx = 0; instIdx < fileLength; instIdx++) {
		instructions[instIdx] = fgetc(f);
	}
	fclose(f);
	
	// Execute instructions
	for (instIdx = 0; instIdx < fileLength; instIdx++) {
		executeInstruction(&memPtr, instructions, &instIdx);
	}

	free(memory);
	free(instructions);

	return 0;
}

void executeInstruction(char** memPtr, char* instruction, int* i) {
	int loop = 1;
	switch(instruction[*i]) {
		case '>': *memPtr += 1; break;
		case '<': *memPtr -= 1; break;
		case '+': **memPtr += 1; break;
		case '-': **memPtr -= 1; break;
		case '.': putchar(**memPtr); break;
		case ',': break; //TODO: Implement instruction
		case '[':
			if (!**memPtr) { // If the data at the pointer is zero, jump to ']'
				while(loop) {
					*i += 1;
					if(instruction[*i] == '[') {
						loop++;
					} else if(instruction[*i] == ']') {
						loop--;
					}
				}
			}
			break;
		case ']':
			if (**memPtr) { // If the data at the pointer is not zero, jump to '['
				while(loop) {
					*i -= 1;
					if(instruction[*i] == ']') {
						loop++;
					} else if(instruction[*i] == '[') {
						loop--;
					}
				}
			}
			break;
		// default:
		// 	fprintf(stderr, "Unknown symbol: '%c'\n", op);
	}
}
