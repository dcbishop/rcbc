#ifndef LL_H
#define LL_H

/**
 * A generic linked list node
 */
typedef struct LLNode {
	void* data;
	struct LLNode* next;
	struct LLNode* prev;
} LLNode;

/**
 * A generic linked list node
 */
typedef struct LL {
	int count;
	struct LLNode* first;
	struct LLNode* last;
} LL;

LL* LLGenerate();
LLNode* LLNodeGenerate(void* data);
LLNode* LLAdd(LL* head, void* data);
LLNode* LLDelete(LLNode* node);
void LLFree(LL* rootnode);

#endif
