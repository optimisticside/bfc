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
	if (curr == NULL || curr->prev == NULL || arguments.optlvl < 1)
		return;
	Node *prev = curr->prev;
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
		// change its type to I_NONE. This is done in other places
		// of this file for the same reason.
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

// Runs several loop optimizations.
void optloop(Node *node) {
	if (node == NULL || node->type != I_LOOP)
		return;
	if (node->childs != NULL) {
		Node *child = node->childs;
		// Optimizes nested loops.
		// An example of this is [[ ... ]] to [ ... ]
		if (arguments.optlvl >= 2 && child->type == I_LOOP && child->next == NULL) {
			node->childs = child->childs;
			// We need to set the childs pointer to NULL,
			// or `rmnode` will remove all of its children.
			child->childs = NULL;
			rmnode(child);
		}
		// Optimizes clear loops, that  do [-] or [+]
		// and can be replaced by a clear instruction.
		if (arguments.optlvl >= 1 && (child->type == I_DEC || child->type == I_INC)
		    && child->next == NULL && !child->data) {
			node->type = I_CLEAR;
			rmnode(child);
		}
	}
	// Optimizes dead loops found at the beginning
	// of programs before any increment or decrement operations.
	if (arguments.optlvl >= 2 && node->parent == NULL) {
		for (Node *curr = node->prev; curr != NULL; curr = curr->prev) {
			if (curr->type == I_INC || curr->type == I_DEC || curr->type == I_INPUT)
				return;
		}
		if (node->prev == NULL)
			node->type = I_NONE;
		else
			rmnode(node);
	}
}

// Optimizes clears before inc/dec operations to just be
// set instructions.
void optclr(Node *node) {
	if (arguments.optlvl < 2 || node == NULL || node->type != I_CLEAR)
		return;
	Node *next = node->next;
	if (next == NULL || (next->type != I_INC && next->type != I_DEC))
		return;
	long val = ((next->type == I_DEC) ? -1 : 1) * (long)(next->data ? next->data : 1);
	// If we remove the next node instead of the current one,
	// we do not have to worry about it being the root node.
	rmnode(next);
	node->type = I_SET;
	node->data = val;
}

// Optimizes operations that don't have any affects
// on the program.
void optjunk(Node *node) {
	if (arguments.optlvl < 2 || node == NULL)
		return;
	// Remove operations before input and clear operations,
	// which will clobber the data.
	if (node->type == I_INPUT || node->type == I_CLEAR) {
		Node *prev = NULL;
		for (Node *curr = node->prev; curr != NULL; curr = prev) {
			prev = curr->prev;
			if (curr->type != I_INC && curr->type != I_DEC
			    && curr->type != I_CLEAR && curr->type != I_INPUT)
				break;
			if (prev == NULL && curr->parent == NULL)
				curr->type = I_NONE;
			else
				rmnode(curr);
		}
	}
}

// Check for a potential infinite loop.
void chkinfloop(Node *node) {
	if (node == NULL || node->type != I_LOOP || arguments.silent)
		return;
	for (Node *child = node->childs; child != NULL; child = child->next) {
		if (child->type == I_DEC || child->type == I_INC)
			return;
		if (child->type == I_PTRINC || child->type == I_PTRDEC)
			return;
	}
	printf("Warning: Potential infinite loop at token %ld (node %p)\n",
		(long)(node->tok->src - _src), node);
}

// Main tree optimization routine.
// Loops through nodes and calls itself recursively
// to descend tree.
void optimize(Node *node) {
	for (Node *curr = node; curr != NULL && curr->type != I_NONE; curr = curr->next) {
		if (curr->type == I_LOOP) {
			optimize(curr->childs);
			optloop(curr);
			chkinfloop(curr);
			continue;
		}
		optincdec(curr, I_PTRINC, I_PTRDEC);
		optincdec(curr, I_INC, I_DEC);
	}
	// Junk and clear optimization depends on the nodes
	// ahead to also be optimized.
	for (Node *curr = node; curr != NULL && curr->type != I_NONE; curr = curr->next) {
		optjunk(curr);
		optclr(curr);
	}
}