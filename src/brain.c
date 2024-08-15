#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "(@): call error (%s)\n", argv[0]);
		return 1;
	}
	FILE *file = fopen(argv[1], "r");
	if (file == NULL) {
		fprintf(stderr, "(@): open error (%s)\n", argv[1]);
		return 1;
	}
	if (fseek(file, 0, SEEK_END) != 0) {
		fprintf(stderr, "(@): seek error (%s)\n", argv[1]);
		return 1;
	}
	int size = ftell(file);
	if (size == -1) {
		fprintf(stderr, "(@): size error (%s)\n", argv[1]);
		return 1;
	}
	if (fseek(file, 0, SEEK_SET) != 0) {
		fprintf(stderr, "(@): seek error (%s)\n", argv[1]);
		return 1;
	}
	char *prog = malloc(size);
	if (prog == NULL) {
		fprintf(stderr, "(@): alloc error (%dB)\n", size);
		return 1;
	}
	if (fread(prog, 1, size, file) < size) {
		fprintf(stderr, "(@): read error (%s)\n", argv[1]);
		return 1;
	}
	if (fclose(file) != 0) {
		fprintf(stderr, "(@): close error (%s)\n", argv[1]);
		return 1;
	}
	int bulk = 1;
	char *tape = malloc(bulk);
	if (tape == NULL) {
		fprintf(stderr, "(@): alloc error (%dB)\n", bulk);
		return 1;
	}
	memset(tape, 0, bulk);
	char *cell = tape;
	for (char *inst = prog; inst < prog + size; inst++) {
		switch (*inst) {
			case '>':
				cell++;
				if (cell >= tape + bulk) {
					char *swap = realloc(tape, bulk * 2);
					if (swap == NULL) {
						fprintf(stderr, "(@): realloc error (%dB)\n", bulk * 2);
						return 1;
					}
					memset(swap + bulk, 0x00, bulk);
					cell += swap - tape;
					tape = swap;
					bulk *= 2;
				}
				break;
			case '<':
				cell--;
				if (cell < tape) {
					char *swap = realloc(tape, bulk * 2);
					if (swap == NULL) {
						fprintf(stderr, "(@): realloc error (%dB)\n", bulk * 2);
						return 1;
					}
					memcpy(swap + bulk, swap, bulk);
					memset(swap, 0x00, bulk);
					cell += swap - tape + bulk;
					tape = swap;
					bulk *= 2;
				}
				break;
			case '+':
				(*cell)++;
				break;
			case '-':
				(*cell)--;
				break;
			case '[':
				if (*cell == 0) {
					for (int loop = 0; inst < prog + size; inst++) {
						if (*inst == '[') {
							loop++;
						} else if (*inst == ']') {
							loop--;
							if (loop == 0) {
								break;
							}
						}
					}
				}
				break;
			case ']':
				if (*cell != 0) {
					for (int loop = 0; inst >= prog; inst--) {
						if (*inst == ']') {
							loop++;
						} else if (*inst == '[') {
							loop--;
							if (loop == 0) {
								break;
							}
						}
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
	return 0;
}
