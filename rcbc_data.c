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
	return thing;
}

RCBCTempory* RCBC_TemporyGenerate() {
	RCBCTempory* tempory = malloc(sizeof(RCBCTempory));
	if(!tempory) {
		return NULL;
	}
	tempory->thing = NULL;
	tempory->sinks = NULL;
	tempory->sources = NULL;
	tempory->unsorted = NULL;
	return tempory;
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
	mesh->triangles = NULL;
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

	debugit(DEBUG_LOW, "%s  RCBC_HookupGenerate: ptr:%p, id:'%s'='%s', pointer:%p", COLOUR_YELLOW, hookup, hookup->id, id, hookup->ptr);

	return hookup;
}

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
	debugit(DEBUG_LOW, "%sRCBC_HookupFree", COLOUR_LIGHT_BLUE);
	LLNode* node_ptr = *roothookup;
	debugit(DEBUG_LOW, "loopbegins...");
	while(node_ptr) {
		debugit(DEBUG_LOW, "\tloop...");
		if(node_ptr->data) {
			//free(((RCBC_Hookup*)node_ptr->data)->id); TODO
			//free(node_ptr->data);
			debugit(DEBUG_LOW, "\thas data...");
			node_ptr->data = NULL;
		}
		node_ptr = node_ptr->next;
	}

	debugit(DEBUG_LOW, "RCBC_HookupFree finish...");
	LLFree(roothookup);
	return;
}

void RCBC_Hookup_Debug(LLNode* rootnode) {
	LLNode* itr;
	RCBC_Hookup* hookup;
	debugit(DEBUG_LOW, "%sRCBC_Hookup_Debug", COLOUR_YELLOW);
	for(itr = rootnode; itr; itr = itr->next) {
		hookup = itr->data;
		debugit(DEBUG_LOW, "ptr: %p, ID: '%s', pointer:%p", hookup, hookup->id, hookup->ptr);
	}
}
void RCBC_Hookup_Execute(LLNode* sources, LLNode* sinks) {
	debugit(DEBUG_LOW, "%sRCBC_Hookup_Execute", COLOUR_LIGHT_BLUE);
	if(sources == NULL) {
		return;
	}
	assert(sources);
	assert(sinks);

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
		if(!destination->ptr) {
			debugit(DEBUG_LOW, "No sink in hookup...");
			continue;
		}
		*destination->ptr = source->ptr;
	}
} 

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

RCBC_TrianglesUnsorted* RCBC_TrianglesUnsortedGenerate(int count) {
	RCBC_TrianglesUnsorted* triangles = malloc(sizeof(RCBC_TrianglesUnsorted));
	if(!triangles) {
		errorit("Falied to malloc memory for linked list node...");
		return NULL;
	}
	triangles->ptr = NULL;
	triangles->count = count;
	triangles->inputs = -1;
	triangles->indices = NULL;
	triangles->vertices = NULL;
	triangles->normals = NULL;
	triangles->textcords = NULL;
	return triangles;
}

int RCBC_TrianglesUnsortedAllocateIndices(RCBC_TrianglesUnsorted* triangles) {
	assert(triangles);
	free(triangles->indices);
	triangles->indices = malloc(sizeof(int) * triangles->count * triangles->inputs * 3);
	if(!triangles->indices) {
		errorit("Failed to allocate index space %d %d %s", triangles->count, triangles->inputs, SYMBOL_WARNING); /*TODO: Fix this message */
		return 1;
	}
	return 0;
}

void RCBC_TrianglesUnsortedFree(RCBC_TrianglesUnsorted* triangles) {
	assert(triangles);
	free(triangles->indices);
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

void LLFree(LLNode** rootnode) {
	if(!rootnode || !*rootnode) {
		return;
	}

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

RCBC_Triangles* RCBC_TrianglesGenerate(int count) {
	RCBC_Triangles* triangles = malloc(sizeof(RCBC_Triangles));
	if(!triangles) {
		errorit("Falied to malloc memory for linked list node...");
		return NULL;
	}

	triangles->count = count;
	triangles->vertices = NULL;
	triangles->normals = NULL;
	triangles->textcords = NULL;
	return triangles;
}

void RCBC_SortTriangles(RCBC_TrianglesUnsorted* unsorted) {
	RCBC_Triangles* triangles = RCBC_TrianglesGenerate(unsorted->count);
	debugit(DEBUG_LOW, "%sRCBC_SortTriangles", COLOUR_LIGHT_BLUE);
	*unsorted->ptr = triangles;
	debugit(DEBUG_LOW, "flag1");
	triangles->count = unsorted->count;

	/* Process vertices */
	if(unsorted->vertices) {
		int v = 0;
		int i;
		//debugit(DEBUG_LOW, "loopbegins.... %d", unsorted->count);
		triangles->vertices = RCBC_FloatArrayGenerate(unsorted->count * 3 * 3);
		for(i = unsorted->vertices_offset; i < 3 * unsorted->count * unsorted->inputs; i+=unsorted->inputs) {
			//debugit(DEBUG_LOW, "loop %d....", i);
			int index = unsorted->indices[i];
			triangles->vertices->values[v++] = unsorted->vertices->values[index];
			triangles->vertices->values[v++] = unsorted->vertices->values[index+2];
			triangles->vertices->values[v++] = unsorted->vertices->values[index+1];
			debugit(DEBUG_LOW, ".... IDX:%d %d:%f %d:%f %d:%f", index, v-2, triangles->vertices->values[v-2], v-1, triangles->vertices->values[v-1], v, triangles->vertices->values[v]);
		}

	}
	debugit(DEBUG_LOW, "flag2");
}
