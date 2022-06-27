#ifndef _TYPES_H_
#define _TYPES_H_

// Instruction types
typedef enum {
	I_NONE,
	I_PTRADD, I_PTRSUB,		// Data pointer operations
	I_ADD, I_SUB, I_CLEAR, I_SET,	// Data operations
	I_OUTPUT, I_INPUT,		// I/O operations
	I_LOOP				// Loop
} InstructionType;

// Token types.
typedef enum {
	TOK_NONE,
	TOK_PTRINC, TOK_PTRDEC,		// Data pointer operations
	TOK_INC, TOK_DEC,		// Data operations
	TOK_INPUT, TOK_OUTPUT,		// I/O operations
	TOK_OPEN, TOK_CLOSE,		// Brackets
} TokenType;

// Lexical token.
typedef struct _Token {
	TokenType type;			// Token type
	char *src;			// Original source
	struct _Token *next;		// Next token in list
} Token;

// Abstract syntax tree node.
typedef struct _Node {
	struct _Node *parent;		// Parent (if existent)
	InstructionType type;		// Node type
	Token *tok;			// Original token
	struct _Node *childs;		// Pointer to first child
	struct _Node *next;		// Next node in list
	struct _Node *prev;		// Previous node in list
	long data;			// Any additional data
} Node;

// Arguments type.
typedef struct _Arguments {
	char *infile;		// Input file
	char *outfile;		// Output file
	int optlvl;		// Optimization level
	int verbose;		// Produce verbose output
	int silent;		// Mute warnings
	int stksize;		// Bracket stack size
	int bufsize;		// Buffer length
} Arguments;

#endif /* !_TYPES_H_ */
