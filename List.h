#ifndef List_H
#define List_H

#include "ooc.h"

/**
 * A generic linked list node.
 */
typedef struct ListNode {
	const ClassFunctions* class_;	/**< Class functions. */
	void* data;						/**< Data stored in the List */
	struct ListNode* next;			/**< Pointer to next node in the List */
	struct ListNode* prev;			/**< Pointer to prev node in the List */
} ListNode;

/**
 * A head Linked list node.
 */
typedef struct List {
	const ClassFunctions* class_;	/**< Class functions. */
	int count;						/**< Number of nodes in the List */
	struct ListNode* first;			/**< Pointer to first node in the List */
	struct ListNode* last;			/**< Pointer to last node in the List */
} List;

void List_0List(List* rootnode);
void ListNode_0ListNode(ListNode* node);
List* List_List();
ListNode* ListNode_ListNode(void* data);
ListNode* ListAdd(List* head, void* data);
void List_DeleteData(List* list);
void List_FreeData(List* list);

#endif
