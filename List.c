#include "List.h"
#include "rcbc_data.h"

#include <stdio.h>
#include <assert.h>

#include "console.h"

/**
 * List deconstructor.
 * Recursivly free a linked list.
 * @param rootnode Pointer to List to free.
 * \memberof List
 */
void List_0List(List* rootnode) {
	assert(rootnode);
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
			ERROR("\tMemory leak!: Data still in deleting list node...");
//			BREAK();
			//free(node_ptr->data);
			//DELETE(node_ptr);
		}
		/*node_ptr->prev = NULL;
		DELETE(node_ptr);*/
		List_DeleteNode(rootnode, node_ptr);
		node_ptr = node_ptr_tmp;
	}
	free(rootnode);
}

/**
 * ListNode deconstructor.
 * Deletes a node from the linked list.
 * @param node Pointer to ListNode to free
 * \memberof ListNode
 */
void ListNode_0ListNode(ListNode* node) {
	DEBUG_M("Entering function...");
	if(node->prev) {
		//node->prev->next = node->next;
	}

	node->prev = NULL;
	node->next = NULL;
	node->data = NULL;
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
 * \memberof List
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
 * \memberof ListNode
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
 * \memberof List
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
 * @see List_FreeData()
 * \memberof List
 */
void List_DeleteData(List* list) {
	DEBUG_M("Entering function...");
	assert(list);
	ListNode* node = list->first;

	while(node) {
		DEBUG_H("\tNode %p...", node);
		DEBUG_H("\tchecking data %p...", node->data);
		if(node->data) {
			DEBUG_H("\t\tDELETE [DATA] %p...", node->data);
			DELETE(node->data);
			//node->data = NULL;
		}
		node->data = NULL;
		node = node->next;
		//DEBUG_H("\tcalling List_DeleteNode...");
		//List_DeleteNode(list, node);
	}
	DEBUG_H("Exiting...");
}

/**
 * Deletes a node from the list, ensuring the list pointers remain correct.
 * @param list The list containing the node.
 * @param node The node to remove from the list.
 * \memberof List
 */
void List_DeleteNode(List* list, ListNode* node) {
	DEBUG_M("Entering function...");
	assert(node);
	assert(list);
	DEBUG_M("\tnode:%p ,listlast:%p, listfirst:%p...", node, list->last, list->first);

	if(list->first == node && list->last == node) {
		DEBUG_H("\tNode is both the first and last node...");
		list->first = NULL;
		list->last = NULL;
	} else {
		DEBUG_H("\tChecking first...");
		if(list->first == node) {
			DEBUG_H("\tnode is first in list...");
			list->first = node->next;
		}
		DEBUG_H("\tChecking last...");
		if(list->last == node) {
			DEBUG_H("\tnode is last in list...");
			list->last = node->prev;
		}
	}
	list->count--;

	DELETE(node);
}

/**
 * Prunes Images without refrences from the list.
 * \memberof List
 */
void List_ScrubImages(List* list) {
	DEBUG_M("Entering function...");

	ListNode* node = list->first;
	while(node) {
		Image* image = node->data;
		int refs = image->refs;
		ListNode* deleteme = node;
		node = node->next;
		if(refs-1 <= 0) {
			DEBUG_H("\tNo more refs for '%s' DELETE()ing...", image->filename);
			if(deleteme->data) {
				DEBUG_H("\tData remains, DELETE()ing...");
				DELETE(deleteme->data);
				deleteme->data = NULL;
			}
			DEBUG_H("\tDeleteNode...");
			List_DeleteNode(list, deleteme);
			DEBUG_H("\tDone with that...");
		}
	}
}

/**
 * free()'s the data from the linked list, for non-complex data structs.
 * Doesn't effect the list itself.
 * @param list List with data to free().
 * @see List_DeleteData()
 * \memberof List
 */
void List_FreeData(List* list) {
	DEBUG_M("Entering function...");
	ListNode* node = list->first;

	while(node) {
		DEBUG_H("\tNode freeing...");
		if(node->data) {
			DEBUG_H("\tFree data...");
			free(node->data);
		}
		node->data = NULL;
		node = node->next;
	}
}

/**
 * Debuggin function.
 * @param list List to dump.
 * \memberof List
 */
void List_DumpList(List* list) {
	DEBUG_M("Entering function...");
	ListNode* node = list->first;
	while(node) {
		DEBUG_H("Node[%p]: node->data: %p");
	}
}


/**
 * Sets all the data pointers in a List to null.
 * No memory freeing or DELETING is done.
 * @param list The list with the data.
 * \memberof List
 */
void List_NullifyData(List* list) {
	ListNode* itr = list->first;
	while(itr) {
		itr->data = NULL;
		itr = itr->next;
	}
}
