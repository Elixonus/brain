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
	int bulk = 1;
	char *tape = malloc(bulk);
	if(tape == NULL)
	{
		fprintf(stderr, "(@): alloc error (%dB)\n", bulk);
		return EXIT_FAILURE;
	}
	memset(tape, 0, bulk);
	char *cell = tape;
	int i = 0;
	for(char *inst = prog; inst < prog + size; inst++)
	{
		switch(*inst)
		{
			case '<':
				cell--;
				if(cell < tape)
				{
					char *swap = realloc(tape, bulk * 2);
					if(swap == NULL)
					{
						fprintf(stderr, "(@): realloc error (%dB)\n", bulk * 2);
						return EXIT_FAILURE;
					}
					memcpy(swap + bulk, swap, bulk);
					cell += swap - tape + bulk;
					tape = swap;
					bulk *= 2;
				}
				break;
			case '>':
				cell++;
				if(cell >= tape + bulk)
				{
					char *swap = realloc(tape, bulk * 2);
					if(swap == NULL)
					{
						fprintf(stderr, "(@): realloc error (%dB)\n", bulk * 2);
						return EXIT_FAILURE;
					}
					cell += swap - tape;
					tape = swap;
					bulk *= 2;
				}
				break;
			case '-':
				(*cell)--;
				if(*cell < 0)
				{
					*cell %= 256;
				}
				break;
			case '+':
				(*cell)++;
				if(*cell >= 256)
				{
					*cell %= 256;
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
				//fprintf(stdout, "number:%d\n", *cell);
				break;
			case ',':
				*cell = fgetc(stdin);
				break;
			default:
				i--;
				break;
		}
		i++;
	}
	printf("\ninstructions:%d\n", i);
	free(prog);
	free(tape);
	return EXIT_SUCCESS;
}
