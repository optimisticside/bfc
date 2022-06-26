#include "decl.h"

// Determines what a token is.
int toktype(char tok) {
	switch (tok) {
	case '>':	return TOK_PTRINC;
	case '<':	return TOK_PTRDEC;
	case '+':	return TOK_INC;
	case '-':	return TOK_DEC;
	case ',':	return TOK_INPUT;
	case '.':	return TOK_OUTPUT;
	case '[':	return TOK_OPEN;
	case ']':	return TOK_CLOSE;
	default:	return TOK_NONE;
	}
}

// Main lexer routine. Converts raw data into token stream.
Token *lex(char *buf) {
	Token *head = NULL, *prev = NULL, *curr;
	char c;

	while ((c = *buf++) != '\0') {
		int type = toktype(c);
		if (type == TOK_NONE)
			continue;
		curr = calloc(1, sizeof(Token));
		if (prev != NULL)
			prev->next = curr;
		if (head == NULL)
			head = curr;
		curr->src = buf - 1;
		curr->type = type;
		prev = curr;
	}

	return head;
}
