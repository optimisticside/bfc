#ifndef _TYPES_H_
#define _TYPES_H_

// Instruction types
typedef enum {
	I_NONE,
	I_PTRINC, I_PTRDEC,		// Data pointer operations
	I_INC, I_DEC,			// Data operations
	I_OUTPUT, I_INPUT,		// IO operations
	I_LOOP				// Loop
} InstructionType;

// Token types.
typedef enum {
	TOK_NONE,
	TOK_PTRINC, TOK_PTRDEC,
	TOK_INC, TOK_DEC,
	TOK_INPUT, TOK_OUTPUT,
	TOK_OPEN, TOK_CLOSE,
} TokenType;

// Lexical token.
typedef struct _Token {
	TokenType type;			// Token type
	char *src;			// Original source
	struct _Token *next;		// Next token in list
} Token;

// Abstract syntax tree node.
typedef struct _Node {
	struct _Node *parent;			// Parent (if existent)
	InstructionType type;			// Node type
	Token *tok;				// Original token
	struct _Node *childs;			// Pointer to first child
	struct _Node *next;			// Next node in list
	struct _Node *prev;			// Previous node in list
	long data;				// Any additional data
} Node;

#endif /* !_TYPES_H_ */
