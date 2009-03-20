#include <assert.h>

#include "rcbc.h"
#include "rcbc_data.h"
#include "console.h"

/* Dump some visual scene node info for debugging */
void RCBC_NodeDebugInfo(RCBCNode* node) {
	DEBUG(DEBUG_VERY_HIGH, "%sRCBC_NodeDebugInfo", COLOUR_LIGHT_BLUE);
	//LLNode* itr = node->rotations->first;
	DEBUG(DEBUG_VERY_HIGH, "Node: Scale %f %f %f", node->scale[0], node->scale[1], node->scale[2]);
	DEBUG(DEBUG_VERY_HIGH, "      Translate %f %f %f", node->translate[0], node->translate[1], node->translate[2]);
	/*for(itr; itr; itr=itr->next) {
		RCBCNode_Rotate* rotation = itr->data;
		DEBUG(DEBUG_VERY_HIGH, "      Rotation %f %f %f %f", rotation->x, rotation->y, rotation->z, rotation->angle);
	}*/
	#warning TODO: Fix this...
}

RCBC_Model* RCBC_ModelGenerate() {
	DEBUG(DEBUG_MEDIUM, "%sRCBC_modelGenerate", COLOUR_LIGHT_BLUE);
	RCBC_Model *model = malloc(sizeof(RCBC_Model));
	if(!model) {
		return NULL;
	}
	model->visual_scene = NULL;
	model->geometries = LLGenerate();
	return model;
}

RCBC_Tempory* RCBC_TemporyGenerate() {
	DEBUG(DEBUG_MEDIUM, "%sRCBC_TemporyGenerate", COLOUR_LIGHT_BLUE);
	RCBC_Tempory* tempory = malloc(sizeof(RCBC_Tempory));
	if(!tempory) {
		return NULL;
	}
	tempory->model = NULL;
	tempory->sinks = LLGenerate();
	tempory->sources = LLGenerate();
	tempory->unsorted = LLGenerate();
	tempory->images = NULL;
	return tempory;
}

void RCBC_ModelFree(RCBC_Model **model) {
	DEBUG(DEBUG_MEDIUM, "%sRCBC_modelFree", COLOUR_LIGHT_BLUE);
	if(!model) {
		return;
	}

	#warning TODO: Free memory recusivly, right now it leaks...
	free(*model);
	*model = NULL;
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

	node->rotations = LLGenerate();

	node->scale[0] = 0.0f;
	node->scale[1] = 0.0f;
	node->scale[2] = 0.0f;

	node->next = NULL;
	node->prev = NULL;
	node->child = NULL;
	node->parent = NULL;
	
	DEBUG(DEBUG_MEDIUM, "end of function");

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
	mesh->arrays = LLGenerate();
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

/* Generates an image */
RCBC_Image* RCBC_ImageGenerate(char* filename) {
	RCBC_Image* image = malloc(sizeof(RCBC_Image));
	if(!image) {
		ERROR("RCBC_MeterialGenerate: Failed to allocate space for material.");
		return NULL;
	}

	image->id = 0;
	image->filename = filename;
	image->refs = 0;

	return image;
}

/* Allocates space for triangles */
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

/**
 * Frees unsorted triangle data.
 */
void RCBC_TrianglesUnsortedFree(RCBC_TrianglesUnsorted* triangles) {
	assert(triangles);
	free(triangles->indices);
	free(triangles);
}

/* Contains vertex data in arrays */
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
			int index = unsorted->indices[i] * 2;
			triangles->texcoords->values[v++] = unsorted->texcoords->values[index];
			triangles->texcoords->values[v++] = unsorted->texcoords->values[index+1];
		}
	}
}
