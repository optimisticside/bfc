#include "decl.h"

// Helper function to add node to stack.
void stkpush(Node **stack, int *stkpos, Node *elem) {
	if ((*stkpos)++ > STKSIZE) {
		fprintf(stderr, "Bracket stack overflow at node %p (type %d, token %ld)\n",
				elem, elem->type, (long)(elem->tok->src - _src));
		exit(-1);
	}
	stack[*stkpos] = elem;
}

// Helper function to remove a node from the stack.
void stkpop(Node **stack, int *stkpos) {
	if ((*stkpos)-- < 0) {
		fprintf(stderr, "Bracket stack underflow (stack size %d)\n",
				*stkpos);
		exit(-1);
	}
}

// Debugs the parser by printing the syntax tree
// in a very simple format.
void printnode(Node *node, int *id) {
	for (; node != NULL && node->type != I_NONE; node = node->next) {
		if (node->type == I_LOOP) {
			printf("%d ", ++(*id));
			printf("\n%d: ", *id);
			printnode(node->childs, id);
		} else
			printf("%c ", *node->tok->src);
	}
	putchar('\n');
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
	int stkpos = 0, descend = 0;
	Node *root = NULL, *prev = NULL, *curr;

	for (; tok != NULL && tok->type != TOK_NONE; tok = tok->next) {
		// We will continue adding nodes in a linked list
		// for adjecent operators. Once we reach the start of a loop
		// we will set the descend flag.
		if (tok->type == TOK_CLOSE) {
			stkpop(stack, &stkpos);
			if (prev != NULL)
				prev = prev->parent;
			continue;
		}
		curr = calloc(1, sizeof(Node));
		curr->prev = prev;
		if (root == NULL)
			root = curr;
		if (prev != NULL) {
			// We can only descend if there is a previous node,
			// for obvious reasons.
			if (descend)
				prev->childs = curr;
			else
				prev->next = curr;
			curr->parent = descend ? prev : prev->parent;
		}
		descend = 0;
		if (tok->type == TOK_OPEN) {
			stkpush(stack, &stkpos, curr);
			descend = 1;
		}
		curr->type = getinst(tok->type);
		curr->tok = tok;
		prev = curr;
	}
	if (stkpos > 0) {
		Node *last = stack[stkpos];
		fprintf(stderr, "Mismatched brackets at node %p (token %ld)\n",
				last, (long)(last->tok->src - _src));
		exit(-1);
	}
	return root;
}
