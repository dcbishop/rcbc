#include <assert.h>

#include "rcbc.h"
#include "rcbc_data.h"
#include "console.h"
#include "GL/gl.h"

#define SWAP(a,b) tmp = a; a = b; b = tmp;
/**
 * Converts to the correct hand rule for OpenGL, deals with various
 * COLLADA axis directions.
 * @param up_axis X_UP, Y_UP or Z_UP depending on COLLADA format.
 * @param x Pointer to the X cordinate to process.
 * @param y Pointer to the Y cordiniate to process.
 * @param z Pointer to the Z cordiniate to process.
 */
void RCBC_FixAxis(const int up_axis, GLfloat *x, GLfloat *y, GLfloat *z) {
	GLfloat tmp;

	// Work out axis mappings
	if(up_axis == X_UP) {
		*y = -*y;
		SWAP(*x, *y);
	} else if (up_axis == Y_UP) {
		// No changes for sane coordinate system
	} else if (up_axis == Z_UP) {
		SWAP(*y, *z);
		*x = -*x;
	}
}

/**
 * Model Deconstructor.
 * @param model Model to free.
 */
void Model_0Model(Model* model) {

	DEBUG_M("Entering function...");

	DEBUG_M("Deleting visual scene %p...", model->visual_scene);
	DELETE(model->visual_scene);

	#warning ['TODO']: Are there any situations where geometires exist but not in visual_scene?
	//DEBUG_M("Deleting geometries...");
	//List_DeleteData(model->geometries);
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
 * @return A new blank model, or NULL.
 */
Model* Model_Model() {
	DEBUG_M("Entering function...");	

	//ALLOCATE(Model, model);
	Model* model = malloc(sizeof(Model));

	model->class_ = &Model_c;

	model->visual_scene = NULL;
	model->geometries = NEW(List);

	return model;
}

/**
 * ModelTempory deconstructor.
 * @param the ModelTempory to free.
 */
void ModelTempory_0ModelTempory(ModelTempory* tempory) {
	DEBUG_M("Entering function...");

	List_DeleteData(tempory->sinks);
	DELETE(tempory->sinks);

	List_DeleteData(tempory->sources);
	DELETE(tempory->sources);

	List_DeleteData(tempory->unsorted);
	DELETE(tempory->unsorted);

	List_DeleteData(tempory->deleteme);
	DELETE(tempory->deleteme);

	List_FreeData(tempory->freeme);
	DELETE(tempory->freeme);

	free(tempory);
}

/**
 * ModelTempory class function binds.
 */
static const ClassFunctions ModelTempory_c = {
	(void*)ModelTempory_0ModelTempory
};

/**
 * ModelTempory constructor.
 * @return A new ModelTempory or NULL on error.
 */
ModelTempory* ModelTempory_ModelTempory() {
	DEBUG_M("Entering function...");

	//ALLOCATE(ModelTempory, tempory);
	ModelTempory* tempory = malloc(sizeof(ModelTempory));

	tempory->class_ = &ModelTempory_c;

	tempory->model = NULL;
	tempory->sinks = NEW(List);
	tempory->sources = NEW(List);
	tempory->unsorted = NEW(List);
	tempory->images = NULL;
	tempory->freeme = NEW(List);
	tempory->deleteme = NEW(List);
	tempory->up_axis = Y_UP;

	DEBUG_M("Exiting function...", COLOUR_LIGHT_BLUE);
	return tempory;
}

/**
 * Rotate deconstructor.
 * @param rotate The Rotate to free.
 */
void Rotate_0Rotate(Rotate* rotate) {
	DEBUG_M("Entering function...");
	free(rotate);
}

/**
 * Rotate class function binds.
 */
static const ClassFunctions Rotate_c = {
	(void*)Rotate_0Rotate
};

/**
 * Rotate constructor.
 * @return a new blank Rotate or NULL on error.
 */
Rotate* Rotate_Rotate() {
	DEBUG_M("Entering function...");
	ALLOCATE(Rotate, rotate);
	rotate->class_ = &Rotate_c;
	rotate->x = 0.0f;
	rotate->y = 0.0f;
	rotate->z = 0.0f;
	rotate->angle = 0.0f;
	return rotate;
}

/**
 * SceneNode deconstructor.
 * @param node The node to free.
 */
void SceneNode_0SceneNode(SceneNode *node) {
	DEBUG_M("Entering function...");
	assert(node);

	#warning ['TODO']: Free SceneNode memory recusivly, right now it leaks...

	DELETE(node->mesh);
	node->mesh = NULL;
	
	DELETE(node->child);
	DELETE(node->next);	

	List_DeleteData(node->rotations);
	DELETE(node->rotations);
	node->rotations = NULL;

	free(node);
}

/**
 * SceneNode class function binds.
 */
static const ClassFunctions SceneNode_c = {
	(void*)SceneNode_0SceneNode
};

/**
 * SceneNode constructor
 * @return A new SceneNode or NULL on error.
 */
SceneNode* SceneNode_SceneNode() {
	DEBUG_M("Entering function...");

	ALLOCATE(SceneNode, node);
	node->class_ = &SceneNode_c;

	node->mesh = NULL;
	node->translate[0] = 0.0f;
	node->translate[1] = 0.0f;
	node->translate[2] = 0.0f;

	node->rotations = NEW(List);

	node->scale[0] = 1.0f;
	node->scale[1] = 1.0f;
	node->scale[2] = 1.0f;

	node->next = NULL;
	node->prev = NULL;
	node->child = NULL;
	node->parent = NULL;

	DEBUG(DEBUG_MEDIUM, "end of function");

	return node;	
}

/**
 * Dump some visual scene node info for debugging.
 * @param node The node to dump.
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
	#warning ['TODO']: Fix this?...
}

/**
 * Mesh deconstructor.
 * @param mesh The mesh to free.
 */
void Mesh_0Mesh(Mesh *mesh) {
	DEBUG_M("Entering function...");
	if(!mesh) {
		return;
	}

	DELETE(mesh->triangles);

	free(mesh);
}

/**
 * Mesh class function binds.
 */
static const ClassFunctions Mesh_c = {
	(void*)Mesh_0Mesh
};

/**
 * Mesh constructor.
 * @return A new blank mesh or NULL on error.
 */
Mesh* Mesh_Mesh() {
	DEBUG_M("Entering function...");

	ALLOCATE(Mesh, mesh);
	mesh->class_ = &Mesh_c;

	mesh->triangles = NULL;
	return mesh;
}

/**
 * FloatArray deconstructor.
 * @param array The float array to free.
 */
void FloatArray_0FloatArray(FloatArray* array) {
	DEBUG_M("Entering function...");
	free(array->values);
	free(array);
}

/**
 * FloatArray class function binds.
 */
static const ClassFunctions FloatArray_c = {
	(void*)FloatArray_0FloatArray
};

/**
 * FloatArray constructor.
 * @param count The number of floats in the array.
 * @return An emprty float array with count elements or NULL on error.
 */
FloatArray* FloatArray_FloatArray(int count) {
	DEBUG_M("Entering function...");
	ALLOCATE(FloatArray, array);

	array->values = calloc(1, count * sizeof(GLfloat));
	if(!array->values) {
		DELETE(array);
		ERROR("Failed to allocate float array values...");
		return NULL;
	}
	array->class_ = &FloatArray_c;
	array->count = count;

	return array;
}

/**
 * A debuging function.
 * @param The array to dump.
 */
void FloatArray_Dump(FloatArray* array) {
	int i;
	DEBUG_H("Float array dumping %d values\n", array->count);
	for(i = 0; i < array->count; i++) {
		printf("%f, ", array->values[i]);
	}
	printf("\n");
}

/**
 * UnsortedTriangles deconstructor.
 * @param triangles Pointer to UnsortedTriangles to free.
 */
void UnsortedTriangles_0UnsortedTriangles(UnsortedTriangles* triangles) {
	DEBUG_M("Entering function...");
	
	#warning ['TODO']: Remove this commented code
	// These should be handled when tempory is nuked
	//DELETE(triangles->vertices);
	//DELETE(triangles->normals);
	//DELETE(triangles->texcoords);
	
	#warning ['TODO']: Do textures get done here?
	//DELETE(triangles->image);
	free(triangles->indices);
	free(triangles);
}

static const ClassFunctions UnsortedTriangles_c = {
	(void*)UnsortedTriangles_0UnsortedTriangles
};

/**
 * UnsortedTriangles constructor.
 * @param count Number of triangles.
 * @return A new empty UnsortedTriangles of specified size, NULL on err.
 */
UnsortedTriangles* UnsortedTriangles_UnsortedTriangles(int count) {
	DEBUG_M("Entering function...");
	ALLOCATE(UnsortedTriangles, triangles);

	triangles->count = count;
	triangles->class_ = &UnsortedTriangles_c;

	return triangles;
}

/**
 * Allocates space for raw COLLADA triangle indices.
 * @trinalges Pointer to UnsortedTriangles that needs space allocated.
 * @return 1 on error or 0 on success.
 */
int UnsortedTrianglesAllocateIndices(UnsortedTriangles* triangles) {
	DEBUG_M("Entering function...");

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
	DEBUG_M("Entering function...");
	if(!image) {
		return;
	}

	#warning ['TODO']: Unload texture data if it exists, should we do this here?...
	image->refs--;
	if(image->refs <= 0) {
		DEBUG_A("Freeing image %s", image->filename);
		//FREE ME
		if(image->filename) {
			free(image->filename);
			image->filename = NULL;
		}
		
		if(image->id != 0) {
			//SOIL_free_image_data(image->id);
			glDeleteTextures( 1, &image->id);
			image->id = 0;
		}
		free(image);
	}
	//free(image);
};	

static const ClassFunctions Image_c = {
	(void*)Image_0Image
};

/**
 * Image Constructor.
 */
Image* Image_Image(char* filename) {
	DEBUG_M("Entering function...");

	ALLOCATE(Image, image);
	image->class_ = &Image_c;
	image->filename = filename;

	return image;
}

/**
 * Finds an Images based on its filename
 */
Image* Image_FindByName(List* images, char* filename) {
	DEBUG_M("Entering function...");
	assert(images);
	assert(filename);
	
	#warning ['TODO']: Remove debug...
	DEBUG_M("Pre first...");
	ListNode* node = images->first;
	#warning ['TODO']: Remove debug...
	DEBUG_M("Got first...");

	while(node) {
		if(strcasecmp( ((Image*)node->data)->filename, filename) == 0 ) {
			return node->data;
		}
		node = node->next;
	}
	return NULL;
}

/**
 * Adds a image to the list of images or returns one already there.
 * Sets or increases the ref count of the image.
 */
Image* Image_Add(List* images, char* filename, int refs) {
	DEBUG_M("Entering function...");
	assert(images);
	assert(filename);

	Image* image = Image_FindByName(images, filename);
	if(image) {
		image->refs += refs;
	} else {
		image = NEW(Image, filename);
		image->refs = refs;
		ListAdd(images, image);
	}

	return image;
}

void Triangles_0Triangles(Triangles* triangles) {
	DEBUG_M("Entering function...");

	if(!triangles) {return;}

	if(triangles->vertices) {
		DELETE(triangles->vertices);
		triangles->vertices = NULL;
	}
	
	if(triangles->normals) {
		DELETE(triangles->normals);
		triangles->normals = NULL;
	}
	
	if(triangles->texcoords) {
		DELETE(triangles->texcoords);
		triangles->texcoords = NULL;
	}
	
	if(triangles->image) {
		DELETE(triangles->image);
		triangles->image = NULL;
	}

	free(triangles);
}

static const ClassFunctions Triangles_c = {
	(void*)Triangles_0Triangles
};

/* Contains vertex data in arrays */
Triangles* Triangles_Triangles(int count) {
	DEBUG_M("Entering function...");

	ALLOCATE(Triangles, triangles);
	triangles->class_ = &Triangles_c;
	triangles->count = count;
	triangles->vertices = NULL;
	triangles->normals = NULL;
	triangles->texcoords = NULL;
	triangles->image = NULL;
	
	return triangles;
}

/**
 * This takes a COLLADA interlaced-indexed model and turns it into vertex arrays.
 */
void RCBC_SortTriangles(ModelTempory* tempory, UnsortedTriangles* unsorted) {
	DEBUG_M("Entering function...");
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
			triangles->vertices->values[v] = unsorted->vertices->values[index++]*-1;
			triangles->vertices->values[v+1] = unsorted->vertices->values[index++];
			triangles->vertices->values[v+2] = unsorted->vertices->values[index];
			RCBC_FixAxis(tempory->up_axis, &triangles->vertices->values[v], &triangles->vertices->values[v+1], &triangles->vertices->values[v+2]);
			v+=3;
		}
	}

	/* Process normals */
	if(unsorted->normals) {
		int v = 0;

		triangles->normals = NEW(FloatArray, unsorted->count * 3 * 3);
		for(i = unsorted->normals_offset; i < 3 * unsorted->count * unsorted->inputs; i+=unsorted->inputs) {
			int index = unsorted->indices[i] * 3;
			triangles->normals->values[v] = -unsorted->normals->values[index++];
			triangles->normals->values[v+1] = unsorted->normals->values[index++];
			triangles->normals->values[v+2] = unsorted->normals->values[index];
			RCBC_FixAxis(tempory->up_axis, &triangles->normals->values[v], &triangles->normals->values[v+1], &triangles->normals->values[v+2]);
			v+=3;
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
