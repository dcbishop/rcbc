#include <assert.h>

#include "rcbc.h"
#include "rcbc_data.h"
#include "console.h"

/**
 * Model Deconstructor.
 */
void *Model_0Model(Model* model) {
	DELETE(model->geometries);
	free(model);
}

/**
 * Model class function binds.
 */
static const ClassFunctions Model_c = {
	(void*)Model_0Model
};

/**
 * Model Constructor.
 */
Model* Model_Model() {
	DEBUG(DEBUG_MEDIUM, "%sModelGenerate", COLOUR_LIGHT_BLUE);
	
	ALLOCATE(Model, model);
	
	model->class = &Model_c;
	
	model->visual_scene = NULL;
	model->geometries = NEW(List);
	
	return model;
}

void* ModelTempory_0ModelTempory(ModelTempory* tempory) {
	DELETE(tempory->sinks);
	DELETE(tempory->sources);
	DELETE(tempory->unsorted);
	free(tempory);
}

static const ClassFunctions ModelTempory_c = {
	(void*)ModelTempory_0ModelTempory
};

ModelTempory* ModelTempory_ModelTempory() {
	DEBUG_M("Entering function...");
	
	ALLOCATE(ModelTempory, tempory);
	
	tempory->class = &ModelTempory_c;
	
	tempory->model = NULL;
	tempory->sinks = NEW(List);
	tempory->sources = NEW(List);
	tempory->unsorted = NEW(List);
	tempory->images = NULL;

	DEBUG_M("Exiting function...", COLOUR_LIGHT_BLUE);
	return tempory;
}

/**
 * Rotate deconstructor.
 */
void Rotate_0Rotate(Rotate* rotate) {
	free(rotate);
}

static const ClassFunctions Rotate_c = {
	(void*)Rotate_0Rotate
};

/**
 * Rotate constructor.
 */
Rotate* Rotate_Rotate() {
	ALLOCATE(Rotate, rotate);
	rotate->class = &Rotate_c;
	rotate->x = 0.0f;
	rotate->y = 0.0f;
	rotate->z = 0.0f;
	rotate->angle = 0.0f;
	return rotate;
}

void SceneNode_0SceneNode(SceneNode *node) {
	DEBUG(DEBUG_MEDIUM, "%sRCBC_NodeFree", COLOUR_LIGHT_BLUE);
	assert(node);

	#warning TODO: Free SceneNode memory recusivly, right now it leaks...
	
	DELETE(node->mesh);
	
	List_DeleteData(node->rotations);
	DELETE(node->rotations);
	
	free(node);
}

static const ClassFunctions SceneNode_c = {
	(void*)SceneNode_0SceneNode
};

SceneNode* SceneNode_SceneNode() {
	DEBUG_M("Entering function...");
	
	ALLOCATE(SceneNode, node);

	node->mesh = NULL;
	node->translate[0] = 0.0f;
	node->translate[1] = 0.0f;
	node->translate[2] = 0.0f;

	node->rotations = NEW(List);

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

/**
 * Dump some visual scene node info for debugging.
 */
void SceneNodeDebugInfo(SceneNode* node) {
	DEBUG(DEBUG_VERY_HIGH, "%sSceneNodeDebugInfo", COLOUR_LIGHT_BLUE);
	//ListNode* itr = node->rotations->first;
	DEBUG(DEBUG_VERY_HIGH, "Node: Scale %f %f %f", node->scale[0], node->scale[1], node->scale[2]);
	DEBUG(DEBUG_VERY_HIGH, "      Translate %f %f %f", node->translate[0], node->translate[1], node->translate[2]);
	/*for(itr; itr; itr=itr->next) {
		Rotate* rotation = itr->data;
		DEBUG(DEBUG_VERY_HIGH, "      Rotation %f %f %f %f", rotation->x, rotation->y, rotation->z, rotation->angle);
	}*/
	#warning TODO: Fix this?...
}

void Mesh_0Mesh(Mesh *mesh) {
	DEBUG(DEBUG_MEDIUM, "%sRCBC_MeshFree", COLOUR_LIGHT_BLUE);
	if(!mesh) {
		return;
	}

	#warning TODO: Free memory recusivly, right now it leaks...
	
	List_DeleteData(mesh->arrays);
	DELETE(mesh->arrays);
	DELETE(mesh->triangles);
	
	free(mesh);
}

static const ClassFunctions Mesh_c = {
	(void*)Mesh_0Mesh
};

Mesh* Mesh_Mesh() {
	DEBUG(DEBUG_MEDIUM, "%sRCBC_MeshGenerate", COLOUR_LIGHT_BLUE);
	
	ALLOCATE(Mesh, mesh);
	mesh->class = &Mesh_c;
	
	mesh->arrays = NEW(List);
	mesh->triangles = NULL;
	return mesh;
}

void FloatArray_0FloatArray(FloatArray* array) {
	free(array->values);
	free(array);
}

static const ClassFunctions FloatArray_c = {
	(void*)FloatArray_0FloatArray
};

FloatArray* FloatArray_FloatArray(int count) {
	
	ALLOCATE(FloatArray, array);
	
	array->values = calloc(1, count * sizeof(float));
	if(!array->values) {
		DELETE(array);
		ERROR("Failed to allocate float array values...");
		return NULL;
	}
	array->class = &FloatArray_c;
	
	return array;
}
void UnsortedTriangleData_0UnsortedTriangleData(UnsortedTriangleData* triangles) {
	free(triangles->indices);
	free(triangles);
}

static const ClassFunctions UnsortedTriangleData_c = {
	(void*)UnsortedTriangleData_0UnsortedTriangleData
};

UnsortedTriangleData* UnsortedTriangleData_UnsortedTriangleData(int count) {
	
	ALLOCATE(UnsortedTriangleData, triangles);
	
	triangles->count = count;
	triangles->class = &UnsortedTriangleData_c;
	
	return triangles;
}

/**
 * Allocates space for raw COLLADA triangle indices.
 */
int UnsortedTriangleDataAllocateIndices(UnsortedTriangleData* triangles) {
	assert(triangles);
	free(triangles->indices);
	triangles->indices = malloc(sizeof(int) * triangles->count * triangles->inputs * 3);
	if(!triangles->indices) {
		ERROR("Failed to allocate index space for unsorted tringles %s", triangles->count, triangles->inputs, SYMBOL_WARNING);
		return 1;
	}
	return 0;
}

void Image_0Image(Image* image) {
	#warning TODO: Unload texture data if it exists, should we do this here?...
	image->refs--;
	if(image->refs <= 0) {
		//FREE ME
		//free(image);
	}	
};	

static const ClassFunctions Image_c = {
	(void*)Image_0Image
};
	
/**
 * Generates an image
 */
Image* Image_Image(char* filename) {

	ALLOCATE(Image, image);
	image->class = &Image_c;
	image->filename = filename;
	
	return image;
}

void Triangles_0Triangles(Triangles* triangles) {
	if(!triangles) {return;}
	
	DELETE(triangles->vertices);
	DELETE(triangles->normals);
	DELETE(triangles->texcoords);
	DELETE(triangles->image);
	
	free(triangles);
}

static const ClassFunctions Triangles_c = {
	(void*)Triangles_0Triangles
};

/* Contains vertex data in arrays */
Triangles* Triangles_Triangles(int count) {

	ALLOCATE(Triangles, triangles);
	triangles->class = &Triangles_c;
	triangles->count = count;

	return triangles;
}

/**
 * This takes a COLLADA interlaced-indexed model and turns it into vertex arrays.
 */
void RCBC_SortTriangles(UnsortedTriangleData* unsorted) {
	DEBUG(DEBUG_MEDIUM, "Entering function...", COLOUR_LIGHT_BLUE);
	int i;
	
	Triangles* triangles = NEW(Triangles, unsorted->count);
	
	*unsorted->ptr = triangles;
	triangles->count = unsorted->count;
	triangles->image = unsorted->image;
	/* Process vertices */
	if(unsorted->vertices) {
		int v = 0;

		triangles->vertices = NEW(FloatArray, unsorted->count * 3 * 3);
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

		triangles->normals = NEW(FloatArray, unsorted->count * 3 * 3);
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

		triangles->texcoords = NEW(FloatArray, unsorted->count * 2 * 3);
		for(i = unsorted->texcoords_offset; i < 3 * unsorted->count * unsorted->inputs; i+=unsorted->inputs) {
			int index = unsorted->indices[i] * 2;
			triangles->texcoords->values[v++] = unsorted->texcoords->values[index];
			triangles->texcoords->values[v++] = unsorted->texcoords->values[index+1];
		}
	}
}
