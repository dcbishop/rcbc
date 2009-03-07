#include <assert.h>

#include "rcbc.h"
#include "rcbc_data.h"
#include "console.h"

void RCBC_NodeDebugInfo(RCBCNode* node) {
	LLNode* itr = node->rotations;
	debugit(DEBUG_ALWAYS, "Node: Scale %f %f %f", node->scale[0], node->scale[1], node->scale[2]);
	debugit(DEBUG_ALWAYS, "      Translate %f %f %f", node->translate[0], node->translate[1], node->translate[2]);
	for(itr = node->rotations; itr; itr=itr->next) {
		RCBCNode_Rotate* rotation = itr->data;
		debugit(DEBUG_ALWAYS, "      Roatation %f %f %f %f", rotation->x, rotation->y, rotation->z, rotation->angle);
	}
}

RCBCThing* RCBC_ThingGenerate() {
	RCBCThing *thing = malloc(sizeof(RCBCThing));
	if(!thing) {
		return NULL;
	}
	thing->visual_scene = NULL;
	thing->geometries = NULL;
	thing->sinks = NULL;
	thing->sources = NULL;
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
	node->mesh = NULL;
	node->translate[0] = 0.0f;
	node->translate[1] = 0.0f;
	node->translate[2] = 0.0f;

	node->rotations = NULL;

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
	mesh->arrays = NULL;
	mesh->sources = NULL;
	mesh->sinks = NULL;
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
	debugit(DEBUG_LOW, "%s  RCBC_HookupGenerate: '%s'", COLOUR_YELLOW, id);
	if(!id) {
		errorit("Tried to generate NULL hookup.");
		return;
	}

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
		debugit(DEBUG_LOW, "%sRCBC_HookupFind(%s'%s'%s)", COLOUR_LIGHT_BLUE, COLOUR_YELLOW, id, COLOUR_LIGHT_BLUE);
	while(node) {
		debugit(DEBUG_LOW, "\tCHECKING: '%s'", ((RCBC_Hookup*)node->data)->id);
		if(node->data 
			&& strcasecmp(((RCBC_Hookup*)node->data)->id, id) == 0)
		{
			debugit(DEBUG_LOW, "\tMATCHING!!!");
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

void RCBC_Hookup_Execute(LLNode* sources, LLNode* sinks) {
	debugit(DEBUG_LOW, "%sRCBC_Hookup_Execute", COLOUR_LIGHT_BLUE);
	RCBC_Hookup* source;
	RCBC_Hookup* destination;
	LLNode* itr;

	debugit(DEBUG_LOW, "\tLoop begiing...");
	for(itr = sources; itr; itr = itr->next) {
		debugit(DEBUG_LOW, "\t\tLoop......");
		source = itr->data;
		if(!source) {
			continue;
		}
		debugit(DEBUG_LOW, "\t\tsearching for '%s'...", source->id);
		destination = RCBC_HookupFind(sinks, source->id);
		if(!destination) {
			errorit("Hookup failed to find sink '%s'");
			continue;
		}
		debugit(DEBUG_LOW, "\t\tfound '%s'...", source->id);
		*destination->ptr = source->ptr;
	}
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

RCBC_Triangles* RCBC_TrianglesGenerate() {
	RCBC_Triangles* triangles = malloc(sizeof(RCBC_Triangles));
	if(!triangles) {
		errorit("Falied to malloc memory for linked list node...");
		return NULL;
	}
	triangles->count = -1;
	triangles->inputs = -1;
	triangles->index = NULL;
	triangles->vertices = NULL;
	triangles->normals = NULL;
	triangles->textcords = NULL;
	return triangles;
}

int RCBC_TrianglesAllocateIndex(RCBC_Triangles* triangles) {
	assert(triangles);
	free(triangles->index);
	triangles->index = malloc(sizeof(int) * triangles->count * triangles->inputs * 3);
	if(!triangles->index) {
		return 1;
	}
	return 0;
}

void RCBC_TrianglesFree(RCBC_Triangles* triangles) {
	assert(triangles);
	free(triangles->index);
	free(triangles);
}

LLNode* LLAdd(LLNode** rootnode, void* data) {
	assert(rootnode);
	assert(data);
	LLNode* newnode = LLGenerate(data);
	if(!(*rootnode)) {
		*rootnode = newnode;
		return newnode;
	}
	/* TODO: Keep track of last node to save walking... */
	LLNode* node_ptr = *rootnode;
	while(node_ptr->next) {
		node_ptr = node_ptr->next;
	}
	node_ptr->next = newnode;

	return newnode;
}

/*void LLJoin(LLNode** rootnode1, LLNode* rootnode2) {
	assert(rootnode1);
	assert(rootnode2);
	debugit(DEBUG_LOW, "flag1...");
	if(!(*rootnode1)) {
		*rootnode1 = rootnode2;
		return;
	}
	debugit(DEBUG_LOW, "flag2...");
	// TODO: keep track of final node for quick insertion
	LLNode* node = *rootnode1;
	debugit(DEBUG_LOW, "flag3... %p", node);
	debugit(DEBUG_LOW, "flag3... %p", node->next);
	while(node->next) {
		debugit(DEBUG_LOW, "LOOP...");
		node = node->next;
	}
	debugit(DEBUG_LOW, "flagX...");
	node->next = rootnode2;
}*/

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
