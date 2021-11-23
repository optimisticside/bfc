#ifndef _DECL_H_
#define _DECL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "types.h"

#define BUFSIZE 3000
#define STKSIZE 30

extern char *_src;

int toktype(char tok);
Token *lex(char *buf);

int stkpush(Node **stack, int *stkpos, Node *elem);
int stkpop(Node **stack, int *stkpos);
InstructionType getinst(TokenType type);
void printnode(Node *node, int *id);
Node *parse(Token *tok);

void rmnode(Node *node);
void optincdec(Node *curr, InstructionType inc, InstructionType dec);
void chkinfloop(Node *node);
void optimize(Node *curr);

void mkbuf(int size, FILE *output);
void mksetup(FILE *output);
void mkcleanup(FILE *output);
void genop(Node *curr, FILE *output);
void genlist(Node *start, int *loopcnt, FILE *output);
void genloop(Node *parent, int *loopcnt, FILE *output);
int gen(Node *root, FILE *output);

#endif /* !_DECL_H_ */
