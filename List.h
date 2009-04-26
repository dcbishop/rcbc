#ifndef List_H
#define List_H

#include "ooc.h"



/**
 * A generic linked list node.
 * @see List
 * @see ListNode_ListNode()
 * @see ListNode_0ListNode()
 */
typedef struct ListNode {
	const ClassFunctions* class_;	/**< Class functions. */
	void* data;						/**< Data stored in the List */
	struct ListNode* next;			/**< Pointer to next node in the List */
	struct ListNode* prev;			/**< Pointer to prev node in the List */
} ListNode;

void ListNode_0ListNode(ListNode* node);
ListNode* ListNode_ListNode(void* data);




/**
 * A head Linked List.
 * @see ListNode
 * @see List_List()
 * @see List_0List()
 * @see ListAdd()
 * @see List_DeleteData()
 * @see List_FreeData()
 */
typedef struct List {
	const ClassFunctions* class_;	/**< Class functions. */
	int count;						/**< Number of nodes in the List */
	struct ListNode* first;			/**< Pointer to first node in the List */
	struct ListNode* last;			/**< Pointer to last node in the List */
} List;

List* List_List();
void List_0List(List* rootnode);
ListNode* ListAdd(List* head, void* data);
void List_DeleteData(List* list);
void List_DeleteNode(List* list, ListNode* node);
void List_ScrubImages(List* list);
void List_FreeData(List* list);
void List_DumpList(List* list);
void List_NullifyData(List* list);

#endif
