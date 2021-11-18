#include "decl.h"

// Helper function to add node to stack.
void stkpush(Node **stack, int *stkpos, Node *elem) {
	if ((*stkpos)++ > STKSIZE) {
		fprintf(stderr, "Stack overflow at node %p (type %d, token %ld)\n",
				elem, elem->type, (long)(elem->tok->src - _src));
		exit(-1);
	}
	stack[*stkpos] = elem;
}

// Helper function to remove a node from the stack.
void stkpop(Node **stack, int *stkpos) {
	if ((*stkpos)-- < 0) {
		fprintf(stderr, "Stack underflow (stack size %d)\n",
				*stkpos);
		exit(-1);
	}
}

// Retrieves an instruction type to correspond
// the given token type.
// Ignores TOK_CLOSE.
InstructionType getinst(TokenType type) {
	switch (type) {
	case TOK_PTRINC:	return I_PTRINC;
	case TOK_PTRDEC:	return I_PTRDEC;
	case TOK_INC:		return I_INC;
	case TOK_DEC:		return I_DEC;
	case TOK_INPUT:		return I_INPUT;
	case TOK_OUTPUT:	return I_OUTPUT;
	case TOK_OPEN:		return I_LOOP;
	default:
		printf("Warning: Invalid token type %d\n",
			type);
		return I_NONE;
	}
}

// Main parsing routine.
// Generates abstract syntax tree from stream of tokens.
Node *parse(Token *tok) {
	// Neighboring nodes will be arranged in a linked list.
	// If a node has children, then it will store the pointer
	// to its linked list of children.
	Node **stack = calloc(STKSIZE, sizeof(Token));
	int stkpos = 0, opening = 0;
	Node *root = NULL, *prev = NULL, *curr;

	for (; tok != NULL && tok->type != TOK_NONE; tok = tok->next) {
		if (tok->type == TOK_CLOSE) {
			stkpop(stack, &stkpos);
			prev = prev->parent;
			continue;
		}
		curr = malloc(sizeof(Node));
		if (prev != NULL) {
			prev->next = curr;
			if (opening)
				prev->childs = curr;
			curr->parent = opening ? prev : prev->parent;
		}
		if (root == NULL)
			root = curr;
		opening = 0;
		if (tok->type == TOK_OPEN) {
			stkpush(stack, &stkpos, curr);
			opening = 1;
		}
		curr->type = getinst(tok->type);
		curr->tok = tok;
		prev = curr;
	}

	return root;
}
