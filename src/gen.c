#include "decl.h"

// General information
#define BUFSIZE 3000	// Buffer size

// Generates the assembly code to create a buffer.
void mkbuf(int size, FILE *output) {
	fprintf(output, "\tbuf: .zero %d\n", size);
}

// Generates code to set up a stack frame.
void mksetup(FILE *output) {
	fprintf(output, "\tpushq %%rbp\n"
			"\tmovq %%rsp, %%rbp\n");
}

void mkcleanup(FILE *output) {
	fprintf(output, "\tmovq %%rbp, %%rsp\n"
			"\tpopq %%rbp\n");
}

// Generates code for basic operations.
void genop(Node *curr, FILE *output) {
	switch (curr->type) {
	default:
		fprintf(stderr, "Invalid node type for node %p (type %d, token %ld)\n",
				curr, curr->type, (long)(curr->tok->src - _src));
		exit(-1);

	case I_INC:	// Increment value at data pointer
		if (curr->data)
			fprintf(output, "\taddb $%ld, (%%rax)\n", curr->data);
		else
			fprintf(output, "\tincb (%%rax)\n");
		break;

	case I_DEC:	// Decrement value at data pointer
		if (curr->data)
			fprintf(output, "\tsubb $%ld, (%%rax)\n", curr->data);
		else
			fprintf(output, "\tdecb (%%rax)\n");
		break;

	case I_PTRINC:	// Increment data pointer
		if (curr->data)
			fprintf(output, "\taddq $%ld, %%rax\n", curr->data);
		else
			fprintf(output, "\tincq %%rax\n");
		break;

	case I_PTRDEC:	// Decrement data pointer
		if (curr->data)
			fprintf(output, "\tsubq $%ld, %%rax\n", curr->data);
		else
			fprintf(output, "\tdecq %%rax\n");
		break;

	case I_INPUT:	// Read character from user
		// We need to move what _getchar gives us
		// into where the data-pointer is pointing to.
		fprintf(output, "\tpushq %%rax\n"
				"\tmovq stdin(%%rip), %%rdi\n"
				"\tcall getc@PLT\n"
				"\tmovq %%rax, %%rbx\n"
				"\tpopq %%rax\n"
				"\tmovb %%bl, (%%rax)\n");
		break;

	case I_OUTPUT:	// Write character to user
		fprintf(output, "\tmovb (%%rax), %%dil\n"
				"\tpushq %%rax\n"
				"\tmovq stdout(%%rip), %%rsi\n"
				"\tcall putc@PLT\n"
				"\tpopq %%rax\n");
		break;
	}
}

// Generates all nodes.
// Calls itself recursively if necessary.
void genlist(Node *start, int *loopcnt, FILE *output) {
	for (Node *curr = start; curr != NULL && curr->type != I_NONE; curr = curr->next) {
		if (curr->type == I_LOOP)
			genloop(curr, loopcnt, output);
		else
			genop(curr, output);
	}
}

// Generates code for a loop.
void genloop(Node *parent, int *loopcnt, FILE *output) {
	int id = (*loopcnt)++;
	fprintf(output, "loop%d:\n", id);
	genlist(parent->childs, loopcnt, output);

	fprintf(output, "\tcmpb $0, (%%rax)\n"
			"\tjnz loop%d\n", id);
}


// Main code-generation routine.
// Takes control as the main program routine,
// and writes to the output file.
int gen(Node *root, FILE *output) {
	// Some basic information:
	// The 3000-byte data table will be allocated
	// in the data segment and zero'd during compile time.
	// We do not currently plan on checking to see if that
	// buffer will overflow or not (we plan on doing this later).
	// Data pointer is %rax.
	fprintf(output, ".section .data\n");
	mkbuf(BUFSIZE, output);
	fprintf(output, "\n.section .text\n"
			".globl main\n"
			"main:\n");
	mksetup(output);
	fprintf(output, "\tlea buf(%%rip), %%rax\n");

	// `loopcnt` helps us create unique positions
	// to jump to for multiple loops.
	int loopcnt = 1;
	genlist(root, &loopcnt, output);

	mkcleanup(output);
	fprintf(output, "\tret");
	return 0;
}
