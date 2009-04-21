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
	DEBUG_M("Entering function...");
	if(!rootnode) {
		DEBUG_H("\tNo root node...");
		return;
	}

	ListNode* node_ptr = rootnode->first;
	ListNode* node_ptr_tmp;
	while(node_ptr) {
		DEBUG_H("\tPointer...");
		node_ptr_tmp = node_ptr->next;
		if(node_ptr->data) {
			DEBUG_H("\tData fround, this is bad as is should be dead already...");
//			BREAK();
			//free(node_ptr->data);
			//DELETE(node_ptr);
		}
		node_ptr->prev = NULL;
		DELETE(node_ptr);
		node_ptr = node_ptr_tmp;
		
	}
	free(rootnode);
}

/**
 * ListNode deconstructor.
 * Deletes a node from the linked list.
 * @param node Pointer to ListNode to free
 */
void ListNode_0ListNode(ListNode* node) {
	DEBUG_M("Entering function...");
	if(node->prev) {
		node->prev->next = node->next;
	}

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
	DEBUG_M("Entering function...");
	//ALLOCATE(List, list);
	List* list = malloc(sizeof(List));

	list->class_ = &List_c;
	list->count = 0;
	list->first = NULL;
	list->last = NULL;

	return list;
}

/**
 * ListNode constructor
 * Generates a linked list node.
 * @param data Data to be stored in the ListNode.
 * @return Pointer to new ListNode or NULL on error.
 */
ListNode* ListNode_ListNode(void* data) {
	DEBUG_M("Entering function...");
	//ALLOCATE(ListNode, node);
	ListNode* node = malloc(sizeof(ListNode));
	if(!node) {
		return NULL;
	}

	node->class_ = &ListNode_c;
	node->data = data;
	node->prev = NULL;
	node->next = NULL;

	return node;
}

/**
 * Adds a new node to the linked list
 * @param head List head node.
 * @param data Data to be stored in the ListNode.
 * @return New ListNode or NULL on error.
 */
ListNode* ListAdd(List* head, void* data) {
	DEBUG_M("Entering function...");
	assert(head);
	assert(data);

	ListNode* newnode = NEW(ListNode, data);
	if(!newnode) {
		ERROR("Failed to allocate space for node... %s", SYMBOL_WARNING);
		return NULL;
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
	DEBUG_M("Entering function...");
	ListNode* node = list->first;

	while(node) {
		DEBUG_H("\tDELETE Node...");
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
	DEBUG_M("Entering function...");
	ListNode* node = list->first;

	while(node) {
		DEBUG_H("\tNode freeing...");
		if(node->data) {
			DEBUG_H("\Free data...");
			free(node->data);
		}
		node->data = NULL;
		node = node->next;
	}
}

/**
 * Debuggin function.
 * @param list List to dump.
 */
void List_DumpList(List* list) {
	DEBUG_M("Entering function...");
	ListNode* node = list->first;
	while(node) {
		DEBUG_H("Node[%p]: node->data: %p");
	}
}
