#include <stdio.h>
#include <stdlib.h>

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
	for(char *inst = prog; inst < prog + size; inst++)
	{
		fprintf("%c", *inst);
	}
	fprintf("\n");

	free(prog);
	free(tape);
	return EXIT_SUCCESS;
}
