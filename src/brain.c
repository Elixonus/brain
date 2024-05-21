#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "(@): call error (%s)\n", argv[0]);
		return EXIT_FAILURE;
	}
	FILE *file = fopen(argv[1], "r");
	if(file == NULL)
	{
		fprintf(stderr, "(@): open error (%s)\n", argv[1]);
		return EXIT_FAILURE;
	}
	if(fseek(file, 0, SEEK_END) != 0)
	{
		fprintf(stderr, "(@): seek error (%s)\n", argv[1]);
		return EXIT_FAILURE;
	}
	int size = ftell(file);
	if(size == -1)
	{
		fprintf(stderr, "(@): size error (%s)\n", argv[1]);
		return EXIT_FAILURE;
	}
	if(fseek(file, 0, SEEK_SET) != 0)
	{
		fprintf(stderr, "(@): seek error (%s)\n", argv[1]);
		return EXIT_FAILURE;
	}
	char *prog = malloc(size);
	if(prog == NULL)
	{
		fprintf(stderr, "(@): alloc error (%dB)\n", size);
		return EXIT_FAILURE;
	}
	if(fread(prog, 1, size, file) < size)
	{
		fprintf(stderr, "(@): read error (%s)\n", argv[1]);
		return EXIT_FAILURE;
	}
	if(fclose(file) != 0)
	{
		fprintf(stderr, "(@): close error (%s)\n", argv[1]);
		return EXIT_FAILURE;
	}
	int bulk = 1000000;
	char *tape = malloc(bulk);
	if(tape == NULL)
	{
		fprintf(stderr, "(@): alloc error (%dB)\n", bulk);
		return EXIT_FAILURE;
	}
	memset(tape, 0, bulk);
	char *cell = tape;
	for(char *inst = prog; inst < prog + size; inst++)
	{
		switch(*inst)
		{
			case '<':
				cell--;
				if(cell < tape)
				{
					cell = tape;
					char *swap = realloc(tape, size * 2);
					if(swap == NULL)
					{
						fprintf(stderr, "(@): realloc error (%dB)\n", size * 2);
						return EXIT_FAILURE;
					}
					memset(swap, 0, size);
					memcpy(swap + size, tape, size);
					cell += swap - tape + size;
					tape = swap;
					size *= 2;
				}
				break;
			case '>':
				cell++;
				if(cell >= tape + size)
				{
					cell = tape + size - 1;
					char *swap = realloc(tape, size * 2);
					if(swap == NULL)
					{
						fprintf(stderr, "(@): realloc error (%dB)\n", size * 2);
						return EXIT_FAILURE;
					}
					memset(swap + size, 0, size);
					memcpy(swap, tape, size);
					cell += swap - tape;
					tape = swap;
					size *= 2;
				}
				break;
			case '-':
				printf("%d | %d\n", cell - tape, bulk);
				(*cell)--;
				if(*cell < 0)
				{
					//*cell %= 256;
				}
				break;
			case '+':
				printf("%d | %d\n", cell - tape, bulk);
				(*cell)++;
				if(*cell >= 256)
				{
					//*cell %= 256;
				}
				break;
			case '[':
				if(*cell == 0)
				{
					while(*inst != ']')
					{
						inst++;
					}
				}
				break;
			case ']':
				if(*cell != 0)
				{
					while(*inst != '[')
					{
						inst--;
					}
				}
				break;
			case '.':
				fputc(*cell, stdout);
				break;
			case ',':
				*cell = fgetc(stdin);
				break;
		}
	}
	free(prog);
	free(tape);
	return EXIT_SUCCESS;
}
