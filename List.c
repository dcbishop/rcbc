#include "List.h"

#include <stdio.h>
#include <assert.h>

#include "console.h"

/**
 * List deconstructor.
 * Recursivly free a linked list.
 * @param rootnode Pointer to List to free.
 */
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

/**
 * ListNode deconstructor.
 * Deletes a node from the linked list.
 * @param node Pointer to ListNode to free
 */
void ListNode_0ListNode(ListNode* node) {
	node->prev->next = node->next;
	node->prev = NULL;
	node->next = NULL;
	free(node);
}

/**
 * List class functions.
 */
static const ClassFunctions List_c = {
	(void*)List_0List
};

/**
 * ListNode class functions.
 */
static const ClassFunctions ListNode_c = {
	(void*)ListNode_0ListNode
};

/**
 * List constructor.
 * Generates a linked list head node.
 * @return Pointer to new List head node or NULL on error.
 */
List* List_List() {
	ALLOCATE(List, list);

	list->class_ = &List_c;
	return list;
}

/**
 * ListNode constructor
 * Generates a linked list node.
 * @param data Data to be stored in the ListNode.
 * @return Pointer to new ListNode or NULL on error.
 */
ListNode* ListNode_ListNode(void* data) {
	ALLOCATE(ListNode, node);

	node->class_ = &ListNode_c;

	node->data = data;
	return node;
}

/**
 * Adds a new node to the linked list
 * @param head List head node.
 * @param data Data to be stored in the ListNode.
 * @return New ListNode or NULL on error.
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
 * DELETE()'s the data from the linked list assuming its an ooc class.
 * Doesn't effect the list itself.
 * @param list List with data to DELETE().
 */
void List_DeleteData(List* list) {
	ListNode* node = list->first;

	while(node) {
		DELETE(node->data);
		node->data = NULL;
		node = node->next;
	}
}

/**
 * free()'s the data from the linked list, for non-complex data structs.
 * Doesn't effect the list itself.
 * @param list List with data to free(). 
 */
void List_FreeData(List* list) {
	ListNode* node = list->first;

	while(node) {
		if(node->data) {
			free(node->data);
		}
		node->data=NULL;
		node = node->next;
	}
}

/**
 * Debuggin function.
 * @param list List to dump.
 */
void List_DumpList(List* list) {
	ListNode* node = list->first;
	while(node) {
		DEBUG_H("Node[%p]: node->data: %p");
	}
}
