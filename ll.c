#include "ll.h"

#include <stdio.h>
#include <assert.h>

#include "console.h"

/**
 * Generates a linked list head node.
 */
LL* LLGenerate() {
	LL* ll = malloc(sizeof(LL));
	if(!ll) {
		ERROR("Failed to allocate memory for linked list head node...");
	}
	
	ll->first = NULL;
	ll->last = NULL;
	ll->count = 0;
	
	return ll;
}
	

/**
 * Generates a linked list node.
 */
LLNode* LLNodeGenerate(void* data) {
	LLNode* node = malloc(sizeof(LLNode));
	if(!node) {
		ERROR("Failed to allocate memory for linked list node...");
		return NULL;
	}
	node->next = NULL;
	node->prev = NULL;
	node->data = data;
	return node;
}

/**
 * Adds a new node to the linked list
 */
LLNode* LLAdd(LL* head, void* data) {
	assert(head);
	assert(data);
	
	LLNode* newnode = LLNodeGenerate(data);
	if(!newnode) {
		ERROR("Failed to allocate space for node... %s", SYMBOL_WARNING);
	}
	
	if(!head->first) {
		head->first = newnode;
		head->last = newnode;
		return newnode;
	}

	newnode->prev = head->last;
	head->last->next = newnode;
	head->last = newnode;
	head->count++;

	return newnode;
}

/* Deletes a node from the linked list */
LLNode* LLDelete(LLNode* node) {
	node->prev->next = node->next;
	node->prev = NULL;
	node->next = NULL;
	free(node);
}

/* Recursivly free a linked list */
void LLFree(LL* rootnode) {
	if(!rootnode) {
		return;
	}

	LLNode* node_ptr = rootnode->first;
	LLNode* node_ptr_tmp;
	while(node_ptr) {
		node_ptr_tmp = node_ptr->next;
		if(node_ptr->data) {
			free(node_ptr);
		}
		node_ptr = node_ptr_tmp;
	}
}
