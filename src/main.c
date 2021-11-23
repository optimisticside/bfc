#include "decl.h"

char *_src;

// Program entry point.
int main(int argc, char *argv[]) {
	if (argc < 3) {
		fprintf(stderr, "Invalid parameters\n");
		return -1;
	}

	FILE *input = fopen(argv[1], "rb");
	fseek(input, 0, SEEK_END);
	long fsiz = ftell(input);
	fseek(input, 0, SEEK_SET);

	char *buf = malloc(fsiz + 1);
	fread(buf, 1, fsiz, input);
	fclose(input);
	buf[fsiz] = '\0';

	_src = buf;
	Token *tok = lex(buf);
	Node *root = parse(tok);
	optimize(root);

	FILE *output = fopen(argv[2], "w+");
	return gen(root, output);
}
