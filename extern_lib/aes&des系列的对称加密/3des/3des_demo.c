#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "3DES.h"

int main(int argc, char* argv[]){

	size_t memory_size;
	int encipher, ncores;
	long i, bytes_read, rest;

	FILE *input, *output;
	unsigned char* memory;

	rest = 0;

	if(argc != 7){
		printf("\nUsage:\n\t%s -e memory_size ncores key input_file output_file (to encipher)\n\t%s -d memory_size ncores key input_file output_file (to decipher)\n", argv[0], argv[0]);
		return 1;
	}

	if(strcmp(argv[1], "-e") && strcmp(argv[1], "-d")){
		printf("\nERROR: Invalid option \"%s\"\n", argv[1]);
		return 2;
	}

	if(strcmp(argv[1], "-e")) encipher = 0;
	else encipher = 1;

	memory_size = strtoul(argv[2], NULL, 0);
	if(memory_size < 24){
		printf("\nERROR: Memory size must have at least 24 bytes\n");
		return 3;
	}

	if(memory_size % 8){
		printf("\nERROR: Memory size must be in the form 8n\n");
		return 4;
	}

	ncores = atoi(argv[3]);
	if(ncores < 1){
		printf("\nERROR: Number of cores must be greater than 0\n");
		return 5;
	}

	if(strlen(argv[4]) != 24){
		printf("\nERROR: Key must have 192 bits. Ex: \"1234567890abcdefghijklmn\"\n");
		return 6;
	}

	input = fopen(argv[5], "rb");
	if(input == NULL){
		printf("\nERROR: Can't open \"%s\"\n", argv[5]);
		return 7;
	}

	output = fopen(argv[6], "wb");
	if(output == NULL){
		printf("\nERROR: Can't write \"%s\"\n", argv[6]);
		fclose(input);
		return 8;
	}

	memory = (unsigned char*) calloc(memory_size, sizeof(unsigned char));
	if(memory == NULL){
		printf("\nERROR: Can't allocate %zu bytes of memory\n", memory_size);
		fclose(input);
		fclose(output);
		return 9;
	}

	/* Divides the input in blocks of fixed size */
	memset(memory, 0, memory_size);
	for(bytes_read = fread(memory, sizeof(unsigned char), memory_size, input);
		bytes_read == (long)memory_size;
		bytes_read = fread(memory, sizeof(unsigned char), memory_size, input)){

		#pragma omp parallel if(ncores > 1) num_threads(ncores)
		{
			#pragma omp for nowait
			for(i = 0; i < (long)memory_size; i += 8){
				/* Process input, each core process a block */
				TripleDES(&memory[i], argv[4], &memory[i], encipher);
			}
		}

		/* Error on write (could be disc full or protected) */
		if(fwrite(memory, sizeof(unsigned char), memory_size, output) != memory_size){
			printf ("\nERROR: Can't write %zu bytes to disc\n", memory_size);
			free(memory);
			fclose(input);
			fclose(output);
			return 10;
		}
	}

	/* Last block */
	if(bytes_read > 0){
		rest =  bytes_read % 8;

		/* Pad with zeros */
		for(i = bytes_read; i < (bytes_read + rest); ++i) memory[i] = 0;

		#pragma omp parallel if (ncores > 1) num_threads(ncores)
		{
			#pragma omp for nowait
			for(i = 0; i < (bytes_read + rest); i += 8){
				/* Process input, each core process a block */
				TripleDES(&memory[i], argv[4], &memory[i], encipher);
			}
		}
	}

	/* Encipher: add padding */
	if(!strcmp(argv[1], "-e")){
		if(bytes_read > 0){
			if(fwrite(memory, sizeof(unsigned char), (bytes_read + rest), output) != (size_t)(bytes_read + rest)){
				printf ("\nERROR: Can't write %ld bytes to disc\n", (bytes_read + rest));
				free(memory);
				fclose(input);
				fclose(output);
				return 11;
			}
		}
	}

	/* Decipher: remove padding */
	else{
		while(!memory[bytes_read -rest -1]) --bytes_read;

		if(fwrite(memory, sizeof(unsigned char), (bytes_read - rest), output) != (size_t)(bytes_read - rest)){
			printf("\nERROR: Can't write %ld bytes to disc\n", bytes_read);
			free(memory);
			fclose(input);
			fclose(output);
			return 12;
		}
	}

	free(memory);
	fclose(input);
	fclose(output);

	return 0;
}
