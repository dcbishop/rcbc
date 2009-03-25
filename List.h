#ifndef List_H
#define List_H

#include "ooc.h"

/**
 * A generic linked list node
 */
typedef struct ListNode {
	const ClassFunctions* class_;
	void* data;
	struct ListNode* next;
	struct ListNode* prev;
} ListNode;

/**
 * A generic linked list node
 */
typedef struct List {
	const ClassFunctions* class_;
	int count;
	struct ListNode* first;
	struct ListNode* last;
} List;

void List_0List(List* rootnode);
ListNode* ListNode_0ListNode(ListNode* node);
List* List_List();
ListNode* ListNode_ListNode(void* data);
ListNode* ListAdd(List* head, void* data);
void List_DeleteData(List* list);
void List_FreeData(List* list);

#endif
