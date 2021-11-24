#include "decl.h"

// Removes a node from the list.
void rmnode(Node *node) {
	if (node->prev != NULL)
		node->prev->next = node->next;
	if (node->next != NULL)
		node->next->prev = node->prev;
	if (node->parent && node->parent->childs == node)
		node->parent->childs = NULL;
	if (node->childs != NULL) {
		for (Node *curr = node->childs; curr != NULL; curr = curr->next)
			rmnode(curr);
	}
	free(node);
}

// Optimizes increment-decrement instruction pairs.
void optincdec(Node *curr, InstructionType inc, InstructionType dec) {
	Node *prev = curr->prev;
	if (prev == NULL)
		return;
	if (curr->type != inc && curr->type != dec)
		return;
	if (prev->type != inc && prev->type != dec)
		return;
	long a = ((prev->type == dec) ? -1 : 1) * (long)(prev->data ? prev->data : 1);
	long b = ((curr->type == dec) ? -1 : 1) * (long)(curr->data ? prev->data : 1);
	long c = a + b;
	rmnode(curr);
	if (c == 0) {
		// In case this previous node is the root node
		// in the list, we will not deallocate it, but rather
		// change its type to I_NONE.
		if (prev->prev == NULL && prev->parent == NULL)
			prev->type = I_NONE;
		else
			rmnode(prev);
		return;
	}
	prev->type = (c > 0) ? inc : dec;
	if (c < 0)
		c = -c;
	prev->data = c;
}

// Optimizes nested loops.
// An example of this is [[ ... ]] to [ ... ]
void optloop(Node *node) {
	if (node->type != I_LOOP || node->childs == NULL)
		return;
	if (node->childs->type != I_LOOP || node->childs->next != NULL)
		return;
	Node *child = node->childs;
	node->childs = child->childs;
	// We need to set the childs pointer to NULL,
	// or `rmnode` will remove all of its children.
	child->childs = NULL;
	rmnode(child);
}

// Check for a potential infinite loop.
void chkinfloop(Node *node) {
	if (node == NULL || node->type != I_LOOP)
		return;
	for (Node *child = node->childs; child != NULL; child = child->next) {
		if (child->type == I_DEC || child->type == I_INC)
			return;
	}
	printf("Warning: Potential infinite loop at token %ld (node %p)\n",
		(long)(node->tok->src - _src), node);
}

// Main tree optimization routine.
// Loops through nodes and calls itself recursively
// to descend tree.
void optimize(Node *curr) {
	for (; curr != NULL && curr->type != I_NONE; curr = curr->next) {
		if (curr->type == I_LOOP) {
			chkinfloop(curr);
			optimize(curr->childs);
			optloop(curr);
			continue;
		}
		optincdec(curr, I_PTRINC, I_PTRDEC);
		optincdec(curr, I_INC, I_DEC);
	}
}