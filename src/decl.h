#ifndef _DECL_H_
#define _DECL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "types.h"
#define BUFSIZE 3000
#define STKSIZE 30

int toktype(char tok);
Token *lex(char *buf);

void stkpush(Node **stack, int *stkpos, Node *elem);
void stkpop(Node **stack, int *stkpos);
InstructionType getinst(TokenType type);
Node *parse(Token *tok);

void mkbuf(FILE *output, int size);
void mksetup(FILE *output);
void mkcleanup(FILE *output);
void genop(Node *curr, FILE *output);
void genlist(Node *start, int *loopcnt, FILE *output);
void genloop(Node *parent, int *loopcnt, FILE *output);
int gen(Node *root, FILE *output);

#endif /* !_DECL_H_ */
