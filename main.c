#include <stdio.h>
#include <stdlib.h>

void executeInstruction(char** memPtr, char* inst, int* i);
int getFileLength(FILE* f);

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
	fileLength = getFileLength(f);

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

int getFileLength(FILE* f) {
	fseek(f,0,SEEK_END);
	int len = ftell(f);
	rewind(f);
	return len;
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
