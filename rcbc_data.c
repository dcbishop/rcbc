#include <assert.h>

#include "rcbc.h"
#include "rcbc_data.h"
#include "console.h"

/* Dump some visual scene node info for debugging */
void RCBC_NodeDebugInfo(RCBCNode* node) {
	DEBUG(DEBUG_VERY_HIGH, "%sRCBC_NodeDebugInfo", COLOUR_LIGHT_BLUE);
	LLNode* itr = node->rotations;
	DEBUG(DEBUG_VERY_HIGH, "Node: Scale %f %f %f", node->scale[0], node->scale[1], node->scale[2]);
	DEBUG(DEBUG_VERY_HIGH, "      Translate %f %f %f", node->translate[0], node->translate[1], node->translate[2]);
	for(itr = node->rotations; itr; itr=itr->next) {
		RCBCNode_Rotate* rotation = itr->data;
		DEBUG(DEBUG_VERY_HIGH, "      Rotation %f %f %f %f", rotation->x, rotation->y, rotation->z, rotation->angle);
	}
}

RCBC_Model* RCBC_ThingGenerate() {
	DEBUG(DEBUG_MEDIUM, "%sRCBC_ThingGenerate", COLOUR_LIGHT_BLUE);
	RCBC_Model *thing = malloc(sizeof(RCBC_Model));
	if(!thing) {
		return NULL;
	}
	thing->visual_scene = NULL;
	thing->geometries = NULL;
	return thing;
}

RCBC_Tempory* RCBC_TemporyGenerate() {
	DEBUG(DEBUG_MEDIUM, "%sRCBC_TemporyGenerate", COLOUR_LIGHT_BLUE);
	RCBC_Tempory* tempory = malloc(sizeof(RCBC_Tempory));
	if(!tempory) {
		return NULL;
	}
	tempory->thing = NULL;
	tempory->sinks = NULL;
	tempory->sources = NULL;
	tempory->unsorted = NULL;
	tempory->images = NULL;
	return tempory;
}

void RCBC_ThingFree(RCBC_Model **thing) {
	DEBUG(DEBUG_MEDIUM, "%sRCBC_ThingFree", COLOUR_LIGHT_BLUE);
	if(!thing) {
		return;
	}

	#warning TODO: Free memory recusivly, right now it leaks...
	free(*thing);
	*thing = NULL;
}

RCBCNode* RCBC_NodeGenerate() {
	DEBUG(DEBUG_MEDIUM, "%sRCBC_NodeGenerate", COLOUR_LIGHT_BLUE);
	RCBCNode* node = malloc(sizeof(RCBCNode));
	if(!node) {
		ERROR("Failed to allocate memory for a node... %s", SYMBOL_WARNING);
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
	DEBUG(DEBUG_MEDIUM, "%sRCBC_NodeFree", COLOUR_LIGHT_BLUE);
	assert(node);

	#warning TODO: Free memory recusivly, right now it leaks...
	free(*node);
	*node = NULL;

}

RCBCMesh* RCBC_MeshGenerate() {
	DEBUG(DEBUG_MEDIUM, "%sRCBC_MeshGenerate", COLOUR_LIGHT_BLUE);
	RCBCMesh* mesh = malloc(sizeof(RCBCMesh));
	if(!mesh) {
		ERROR("Failed to allocate memory for mesg.");
		return NULL;
	}
	mesh->arrays = NULL;
	mesh->triangles = NULL;
	return mesh;
}

void RCBC_MeshFree(RCBCMesh **mesh) {
	DEBUG(DEBUG_MEDIUM, "%sRCBC_MeshFree", COLOUR_LIGHT_BLUE);
	if(!mesh) {
		return;
	}

	#warning TODO: Free memory recusivly, right now it leaks...
	free(*mesh);
	*mesh = NULL;
}

/* Allocates a hookup */
RCBC_Hookup* RCBC_HookupGenerate(char* id, void* pointer) {
	DEBUG(DEBUG_MEDIUM, "Entering function...");
	if(!id) {
		WARNING("Tried to generate NULL hookup.");
		return;
	}

	RCBC_Hookup* hookup = malloc(sizeof(RCBC_Hookup));
	if(!hookup) {
		ERROR("Failed to allocate space for hookup.");
		return NULL;
	}

	DEBUG_H("Values: ID:'%s', pointer:%p", id, pointer);

	hookup->id = id;
	hookup->ptr = pointer;
	DEBUG_H("exiting function...");

	return hookup;
}

RCBC_Hookup* RCBC_HookupFind(LLNode* roothookup, char* id) {
	LLNode* node = roothookup;
	DEBUG(DEBUG_MEDIUM, "%sRCBC_HookupFind(%s'%s'%s)", COLOUR_LIGHT_BLUE, COLOUR_YELLOW, id, COLOUR_LIGHT_BLUE);
	while(node) {
		DEBUG(DEBUG_VERY_HIGH, "\tCHECKING: '%s'", ((RCBC_Hookup*)node->data)->id);
		if(node->data 
			&& strcasecmp(((RCBC_Hookup*)node->data)->id, id) == 0)
		{
			DEBUG(DEBUG_HIGH, "\tFound matching node %s", SYMBOL_SMILEY);
			return node->data;
		}

		node = node->next;
	}

	return NULL;
}

/* Recursivly frees all the hookups */
void RCBC_HookupFree(LLNode** roothookup) {
	DEBUG(DEBUG_MEDIUM, "%sRCBC_HookupFree", COLOUR_LIGHT_BLUE);
	LLNode* node_ptr = *roothookup;
	DEBUG(DEBUG_HIGH, "\tLooping through each hookup...");
	while(node_ptr) {
		DEBUG(DEBUG_VERY_HIGH, "\t\tloop...");
		if(node_ptr->data) {
			//free(((RCBC_Hookup*)node_ptr->data)->id); #warning TODO:s free hookups
			//free(node_ptr->data);
			DEBUG(DEBUG_VERY_HIGH, "\t\tNode has data...");
			node_ptr->data = NULL;
		}
		node_ptr = node_ptr->next;
	}

	DEBUG(DEBUG_LOW, "RCBC_HookupFree finish...");
	LLFree(roothookup);
	return;
}

void RCBC_Hookup_Debug(LLNode* rootnode) {
	LLNode* itr;
	RCBC_Hookup* hookup;
	DEBUG(DEBUG_MEDIUM, "%sRCBC_Hookup_Debug", COLOUR_YELLOW);
	for(itr = rootnode; itr; itr = itr->next) {
		hookup = itr->data;
		DEBUG(DEBUG_VERY_HIGH, "ptr: %p, ID: '%s', pointer:%p", hookup, hookup->id, hookup->ptr);
	}
}
void RCBC_Hookup_Execute(LLNode* sources, LLNode* sinks) {
	DEBUG_M("Entering function...");
	if(sources == NULL) {
		return;
	}
	assert(sources);
	assert(sinks);

	RCBC_Hookup* source;
	RCBC_Hookup* destination;
	LLNode* itr;

	DEBUG_V("\tLoop begiing...");
	for(itr = sources; itr; itr = itr->next) {
		DEBUG(DEBUG_VERY_HIGH, "\t\tLoop......");
		source = itr->data;
		if(!source) {
			continue;
		}
		DEBUG_V("\t\tsearching for '%s'...", source->id);
		destination = RCBC_HookupFind(sinks, source->id);
		if(!destination) {
			#warning TODO: Change error to a warning...
			ERROR("Hookup failed to find sink '%s'", source->id);
			continue;
		}
		DEBUG_H("\t\tfound '%s'...", source->id);
		if(!destination->ptr) {
			DEBUG(DEBUG_HIGH, "No sink in hookup...");
			continue;
		}
		*destination->ptr = source->ptr;
	}
} 

RCBC_FloatArray* RCBC_FloatArrayGenerate(int count) {
	RCBC_FloatArray* array = malloc(sizeof(RCBC_FloatArray));
	if(!array) {
		ERROR("Failed to allocate float array...");
		return NULL;
	}
	array->values = malloc(count * sizeof(float));
	if(!array->values) {
		free(array);
		ERROR("Failed to allocate float array values...");
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
		ERROR("Failed to allocate memory for linked list node...");
		return NULL;
	}
	node->next = NULL;
	node->data = data;
	return node;
}

RCBC_TrianglesUnsorted* RCBC_TrianglesUnsortedGenerate(int count) {
	RCBC_TrianglesUnsorted* triangles = malloc(sizeof(RCBC_TrianglesUnsorted));

	if(!triangles) {
		ERROR("Falied to malloc memory for linked list node...");
		return NULL;
	}

	triangles->ptr = NULL;
	triangles->count = count;
	triangles->inputs = -1;
	triangles->indices = NULL;
	triangles->vertices = NULL;
	triangles->normals = NULL;
	triangles->texcoords = NULL;
	triangles->image = NULL;
	return triangles;
}

RCBC_Image* RCBC_ImageGenerate(char* filename) {
	RCBC_Image* image = malloc(sizeof(RCBC_Image));
	if(!image) {
		ERROR("RCBC_MeterialGenerate: Failed to allocate space for material.");
		return NULL;
	}

	image->filename = filename;
	image->id = 0;	

	return image;
}

int RCBC_TrianglesUnsortedAllocateIndices(RCBC_TrianglesUnsorted* triangles) {
	assert(triangles);
	free(triangles->indices);
	triangles->indices = malloc(sizeof(int) * triangles->count * triangles->inputs * 3);
	if(!triangles->indices) {
		ERROR("Failed to allocate index space for unsorted tringles %s", triangles->count, triangles->inputs, SYMBOL_WARNING);
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

	#warning TODO: Keep track of last node to save walking...
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
		ERROR("Falied to malloc memory for linked list node...");
		return NULL;
	}

	triangles->count = count;
	triangles->vertices = NULL;
	triangles->normals = NULL;
	triangles->texcoords = NULL;
	triangles->image = NULL;

	return triangles;
}

/* This takes a COLLADA interlaced-indexed model and turns it into vertex arrays */
void RCBC_SortTriangles(RCBC_TrianglesUnsorted* unsorted) {
	int i = 0xDEADC0DE;
	RCBC_Triangles* triangles = RCBC_TrianglesGenerate(unsorted->count);
	DEBUG(DEBUG_MEDIUM, "%sRCBC_SortTriangles", COLOUR_LIGHT_BLUE);
	*unsorted->ptr = triangles;
	triangles->count = unsorted->count;
	triangles->image = unsorted->image;
	/* Process vertices */
	if(unsorted->vertices) {
		int v = 0;

		triangles->vertices = RCBC_FloatArrayGenerate(unsorted->count * 3 * 3);
		for(i = unsorted->vertices_offset; i < 3 * unsorted->count * unsorted->inputs; i+=unsorted->inputs) {
			int index = unsorted->indices[i] * 3;
			triangles->vertices->values[v++] = -unsorted->vertices->values[index];
			triangles->vertices->values[v++] = unsorted->vertices->values[index+2];
			triangles->vertices->values[v++] = unsorted->vertices->values[index+1];
		}
	}

	/* Process normals */
	if(unsorted->normals) {
		int v = 0;

		triangles->normals = RCBC_FloatArrayGenerate(unsorted->count * 3 * 3);
		for(i = unsorted->normals_offset; i < 3 * unsorted->count * unsorted->inputs; i+=unsorted->inputs) {
			int index = unsorted->indices[i] * 3;
			triangles->normals->values[v++] = -unsorted->normals->values[index];
			triangles->normals->values[v++] = unsorted->normals->values[index+2];
			triangles->normals->values[v++] = unsorted->normals->values[index+1];
		}
	}

	/* Process normals */
	if(unsorted->texcoords) {
		int v = 0;

		triangles->texcoords = RCBC_FloatArrayGenerate(unsorted->count * 2 * 3);
		for(i = unsorted->texcoords_offset; i < 3 * unsorted->count * unsorted->inputs; i+=unsorted->inputs) {
			int index = unsorted->indices[i] * 3;
			triangles->texcoords->values[v++] = -unsorted->texcoords->values[index];
			triangles->texcoords->values[v++] = unsorted->texcoords->values[index+1];
		}
	}
}
