#include <assert.h>

#include "rcbc.h"
#include "rcbc_data.h"
#include "console.h"

void RCBC_NodeDebugInfo(RCBCNode* node) {
	debugit(DEBUG_ALWAYS, "Node: Scale %f %f %f", node->scale[0], node->scale[1], node->scale[2]);
	debugit(DEBUG_ALWAYS, "      Translate %f %f %f", node->translate[0], node->translate[1], node->translate[2]);
}

RCBCThing* RCBC_ThingGenerate() {
	RCBCThing *thing = malloc(sizeof(RCBCThing));
	if(!thing) {
		return NULL;
	}
	thing->visual_scene = NULL;
	thing->geometries = NULL;
	return thing;
}

void RCBC_ThingFree(RCBCThing **thing) {
	if(!thing) {
		return;
	}

	/* TODO: Free memory recusivly, right now it leaks... */
	free(*thing);
	*thing = NULL;
}

RCBCNode* RCBC_NodeGenerate() {
	RCBCNode* node = malloc(sizeof(RCBCNode));
	if(!node) {
		errorit("Failed to allocate memory for a node... %s", SYMBOL_WARNING);
		return NULL;
	}

	node->translate[0] = 0.0f;
	node->translate[1] = 0.0f;
	node->translate[2] = 0.0f;

	node->rotate = NULL;

	node->scale[0] = 0.0f;
	node->scale[1] = 0.0f;
	node->scale[2] = 0.0f;

	node->next = NULL;
	node->prev = NULL;
	node->child = NULL;
	node->parent = NULL;

	return node;	
}

void RCBC_NodeFree(RCBCNode **node) {
	assert(node);

	/* TODO: Free memory recusivly, right now it leaks... */
	free(*node);
	*node = NULL;

}

RCBCMesh* RCBC_MeshGenerate() {
	RCBCMesh* mesh = malloc(sizeof(RCBCMesh));
	if(!mesh) {
		errorit("Failed to allocate memory for mesg.");
		return NULL;
	}

	return mesh;
}

void RCBC_MeshFree(RCBCMesh **mesh) {
	if(!mesh) {
		return;
	}

	/* TODO: Free memory recusivly, right now it leaks... */
	free(*mesh);
	*mesh = NULL;
}

/* Allocates a hookup */
RCBC_Hookup* RCBC_HookupGenerate(char* id, void** pointer) { 
	RCBC_Hookup* hookup = malloc(sizeof(RCBC_Hookup));
	if(!hookup) {
		errorit("Failed to allocate space for hookup.");
		return NULL;
	}

	hookup->id = id;
	hookup->ptr = pointer;

	return hookup;
}

/* Adds a new hookup to the list */
/*RCBC_Hookup* RCBC_HookupAdd(LLNode** roothookup, int type, char* id, void* pointer) {
	RCBC_Hookup* last;
	if(!roothookup || !pointer) {
		return;
	}

	if(!(*roothookup)) {
		*roothookup = RCBC_HookupGenerate(type, id, pointer);
		last = *roothookup;
	} else {
		// Loop until end node
		for(last = (*roothookup); last->next; last = last->next) {	debugit(DEBUG_LOW, "\twalking..."); }
		last->next = TemporyHookup_Generate(type, id, pointer);
		last = last->next;
	}
}*/

RCBC_Hookup* RCBC_HookupFind(LLNode* roothookup, char* id) {
	LLNode* node = roothookup;
	while(node) {

		if(!node->data 
			&& strcasecmp(((RCBC_Hookup*)node->data)->id, id) == 0)
		{
			return node->data;
		}

		node = node->next;
	}

	return NULL;
}

/* Recursivly frees all the hookups */
void RCBC_HookupFree(LLNode** roothookup) {

	LLNode* node_ptr;
	while(node_ptr) {
		if(node_ptr->data) {
			free(((RCBC_Hookup*)node_ptr->data)->id);
			free(node_ptr->data);
			node_ptr->data = NULL;
		}
		node_ptr = node_ptr->next;
	}

	LLFree(roothookup);
	return;
}

/*void TemporyHookup_Hookup(TemporyHookup* roothookup) {
	TemporyHookup* sink;
	TemporyHookup* hookup = roothookup;
	while(hookup) {
		if(hookup->type == HOOKUP_ID2PTR) {
			sink = TemporyHookup_Find(roothookup, hookup->id);
		}
		hookup = hookup->next;
	}
}*/

RCBC_FloatArray* RCBC_FloatArrayGenerate(int count) {
	RCBC_FloatArray* array = malloc(sizeof(RCBC_FloatArray));
	if(!array) {
		errorit("Failed to allocate float array...");
		return NULL;
	}
	array->values = malloc(count * sizeof(float));
	if(!array->values) {
		free(array);
		errorit("Failed to allocate float array values...");
		return NULL;
	}
	int i;
	for(i = 0; i < count; i++) {
		array->values[i] = 0.0f;
	}

	return array;
}

void RCBC_FloatArrayFree(RCBC_FloatArray* array) {
	assert(array);
	free(array->values);
	free(array);
}

LLNode* LLGenerate(void* data) {
	LLNode* node = malloc(sizeof(LLNode));
	if(!node) {
		errorit("Failed to allocate memory for linked list node...");
		return NULL;
	}
	node->next = NULL;
	node->data = data;
	return node;
}

LLNode* LLAdd(LLNode** rootnode, void* data) {
	assert(rootnode);
	assert(data);
	LLNode* newnode = LLGenerate(data);
	if(!(*rootnode)) {
		*rootnode = newnode;
	}
	return newnode;
}

void LLFree(LLNode** rootnode) {
	assert(rootnode);
	LLNode* node_ptr;
	LLNode* node_ptr_tmp;
	while(node_ptr) {
		node_ptr_tmp = node_ptr->next;
		if(node_ptr->data) {
			free(node_ptr);
		}
		node_ptr = node_ptr_tmp;
	}
}
