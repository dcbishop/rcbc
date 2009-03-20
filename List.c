#include "List.h"

#include <stdio.h>
#include <assert.h>

#include "console.h"

/* Recursivly free a linked list */
void List_0List(List* rootnode) {
	if(!rootnode) {
		return;
	}

	ListNode* node_ptr = rootnode->first;
	ListNode* node_ptr_tmp;
	while(node_ptr) {
		node_ptr_tmp = node_ptr->next;
		if(node_ptr->data) {
			free(node_ptr);
		}
		node_ptr = node_ptr_tmp;
	}
}

/* Deletes a node from the linked list */
ListNode* ListNode_0ListNode(ListNode* node) {
	node->prev->next = node->next;
	node->prev = NULL;
	node->next = NULL;
	free(node);
}

/**
 * List class
 */
static const ClassFunctions List_c = {
	(void*)List_0List
};

/**
 * List node class
 */
static const ClassFunctions ListNode_c = {
	(void*)ListNode_0ListNode
};

/**
 * Generates a linked list head node.
 */
List* List_List() {
	ALLOCATE(List, list);
	
	list->class = &List_c;
	return list;
}
	

/**
 * Generates a linked list node.
 */
ListNode* ListNode_ListNode(void* data) {
	ALLOCATE(ListNode, node);
	
	node->class = &ListNode_c;

	node->data = data;
	return node;
}

/**
 * Adds a new node to the linked list
 */
ListNode* ListAdd(List* head, void* data) {
	assert(head);
	assert(data);
	
	ListNode* newnode = NEW(ListNode, data);
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

/**
 * Deletes the data from the linked list assuming its an ooc class.
 */
void* List_DeleteData(List* list) {
	ListNode* node = list->first;

	while(node) {
		DELETE(node->data);
		node->data = NULL;
		node = node->next;
	}
}
