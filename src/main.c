#include "decl.h"

char *_src;
Arguments arguments = { NULL, NULL, 0, 0 };
static char *argdoc = "INFILE OUTFILE";
static char *doc = "Compiles a Brainf**k program to assembly.";
static struct argp_option options[] = {
	{ "verbose", 'v', 0, 0, "Produce verbose output" },
	{ 0, 'O', "OPTLVL", OPTION_ARG_OPTIONAL, "Optimization level (0 to 2)" },
	{ "output", 'o', "OUTFILE", 0, "File to write compiled code to" },
	{ 0 }
};

int parseopt(int key, char *arg, struct argp_state *state) {
	Arguments *args = state->input;
	switch (key) {
	default:
		return ARGP_ERR_UNKNOWN;
	case 'O':
		args->optlvl = atoi(arg);
		if (args->optlvl < 0 || args->optlvl > 2) {
			args->optlvl = 0;
			return ARGP_ERR_UNKNOWN;
		}
		break;
	case 'v':
		args->verbose = 1;
		break;
	case 'o':
		args->outfile = arg;
		break;
	case ARGP_KEY_ARG:
		//if (state->arg_num > ARGCOUNT)
		//	argp_usage(state);
		args->infile = arg;
		break;
	case ARGP_KEY_END:
		//if (state->arg_num < ARGCOUNT)
		//	argp_usage(state);
		break;
	}
	return 0;
}

// Program entry point.
int main(int argc, char *argv[]) {
	struct argp argp = {options, &parseopt, argdoc, doc};
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	if (arguments.infile == NULL) {
		fprintf(stderr, "No input files provided.\n");
		return -1;
	}
	if (arguments.outfile == NULL) {
		fprintf(stderr, "No output file provided.\n");
		return -1;
	}

	FILE *input = fopen(arguments.infile, "rb");
	if (input == NULL) {
		fprintf(stderr, "Unable to open file '%s'\n", arguments.infile);
		return -1;
	}
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

	FILE *output = fopen(arguments.outfile, "w+");
	if (output == NULL) {
		fprintf(stderr, "Unable to create output file '%s'\n"
				"Check the directory permissions.\n", arguments.outfile);
		return -1;
	}
	return gen(root, output);
}
