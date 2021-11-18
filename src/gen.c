#include "decl.h"

// General information
#define BUFSIZE 3000	// Buffer size

// Generates the assembly code to create a buffer.
void mkbuf(FILE *output, int size) {
	fprintf(output, "buf:\n\t.zero %d", size);
}

// Generates code to set up a stack frame.
void mksetup(FILE *output) {
	fprintf(output, "\tpushq %%rbp\n"
			"\tmovq %%rsp, %%rbp");
}

void mkcleanup(FILE *output) {
	fprintf(output, "\tmovq %%rbp, %%rsp\n"
			"\tpopq %%rbp");
}

// Generates code for basic operations.
void genop(Node *curr, FILE *output) {
	switch (curr->type) {
	default:
		fprintf(stderr, "Invalid node type for node %p\n", curr);
		exit(-1);

	case I_INC:	// Increment value at data pointer
		fprintf(output, "\tincq (%%rax)\n");
		break;

	case I_DEC:	// Decrement value at data pointer
		fprintf(output, "\tdecq (%%rax)\n");
		break;

	case I_PTRINC:	// Increment data pointer
		fprintf(output, "\tincq %%rax\n");
		break;

	case I_PTRDEC:	// Decrement data pointer
		fprintf(output, "\tdecq %%rax\n");
		break;

	case I_INPUT:	// Read character from user
		// We need to move what _getchar gives us
		// into where the data-pointer is pointing to.
		fprintf(output, "\tpushq %%rax\n"
				"\tcall _getchar\n"
				"\tmovq %%rax, %%rbx"
				"\tpopq %%rax"
				"\tmovq %%rbx, (%%rax)");
		break;

	case I_OUTPUT:	// Write character to user
		fprintf(output, "\tmovq %%rax, %%rbx\n"
				"\t movq (%%rbx), %%rax"
				"\tpushq %%rbx"
				"\tcall _putchar\n"
				"\tpopq %%rax");
		break;
	}
}

// Generates all nodes.
// Calls itself recursively if necessary.
void genlist(Node *start, int *loopcnt, FILE *output) {
	for (Node *curr = start; curr != NULL; curr = curr->next) {
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

	fprintf(output, "\tcmp 0, %%eax"
			"\tjle loop%d", id);
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
	fprintf(output, ".data");
	mkbuf(output, BUFSIZE);
	fprintf(output, ".text\n_start:\n");
	mksetup(output);

	// `loopcnt` helps us create unique positions
	// to jump to for multiple loops.
	int loopcnt = 1;
	genlist(root, &loopcnt, output);

	mkcleanup(output);
	fprintf(output, "\tret");
	return 0;
}
