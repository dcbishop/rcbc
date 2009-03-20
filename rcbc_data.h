/* Contains COLLADA data scructures */
#ifndef _RCBC_DATA_DEF
#define _RCBC_DATA_DEF

#include "List.h"
#include "rcbc_data_hookup.h"

/**
 * A container for a COLLADA model.
 */
typedef struct Model {
	const ClassFunctions* class;
	struct SceneNode* visual_scene;
	struct List* geometries;
} Model;

/**
 * A throwaway struct for holding tempory values while loading a model.
 */
typedef struct ModelTempory {
	const ClassFunctions* class;
	Model* model;
	List* sources;
	List* sinks;
	List* unsorted;
	List* images;
} ModelTempory;

/**
 *  Contains rotation information for a COLLADA scene node.
 **/
typedef struct Rotate {
	const ClassFunctions* class;
	float x;
	float y;
	float z;
	float angle;
} Rotate;

typedef struct FloatArray {
	const ClassFunctions* class;
	int count;
	float* values;
} FloatArray;

/** 
 * Contains a filename for a image, an id for opengl texture binding 
 * and the number of refrences to this image.
 */
typedef struct Image {
	const ClassFunctions* class;
	int id;
	char* filename;
	int refs;
} Image;

/**
 * Contains unprocessed triangle data as it is read from the COLLADA
 * file.
 */
typedef struct UnsortedTriangles {
	const ClassFunctions* class;
	void** ptr;
	unsigned int count;
	unsigned int inputs;
	unsigned int* indices;
	int vertices_offset;
	FloatArray* vertices;
	int normals_offset;
	FloatArray* normals;
	int texcoords_offset;
	FloatArray* texcoords;
	Image* image;
} UnsortedTriangles;

typedef struct UnsortedPolygons {
	const ClassFunctions* class;
	void** ptr;
	unsigned int count;
	unsigned int inputs;
} UnsortedPolygons;

typedef struct Triangles {
	const ClassFunctions* class;
	unsigned int count;
	FloatArray* vertices;
	FloatArray* normals;
	FloatArray* texcoords;
	Image* image;
} Triangles;

/** 
 * Contains mesh data
 **/
typedef struct Mesh {
	const ClassFunctions* class;
	List* arrays;
	Triangles* triangles;
} Mesh;

/**
 * A basic COLLADA scene node
 */
typedef struct SceneNode {
	const ClassFunctions* class;
	
	Mesh* mesh;

	float translate[3];
	float scale[3];

	struct List* rotations;

	struct SceneNode* next;
	struct SceneNode* prev;
	struct SceneNode* child;
	struct SceneNode* parent;
} SceneNode;


void *Model_0Model(Model* model);
Model* Model_Model();

void* ModelTempory_0ModelTempory(ModelTempory* tempory);
ModelTempory* ModelTempory_ModelTempory();

void Rotate_0Rotate(Rotate* rotate);
Rotate* Rotate_Rotate();

void SceneNode_0SceneNode(SceneNode *node);
SceneNode* SceneNode_SceneNode();
void SceneNodeDebugInfo(SceneNode* node);

void Mesh_0Mesh(Mesh *mesh);
Mesh* Mesh_Mesh();

void FloatArray_0FloatArray(FloatArray* array);
FloatArray* FloatArray_FloatArray(int count);

void UnsortedTriangles_0UnsortedTriangles(UnsortedTriangles* triangles);
UnsortedTriangles* UnsortedTriangles_UnsortedTriangles(int count);
int UnsortedTrianglesAllocateIndices(UnsortedTriangles* triangles);

void Image_0Image(Image* image);
Image* Image_Image(char* filename);
Image* Image_FindByName(List* images, char* filename);
Image* Image_Add(List* images, char* filename, int refs);

void Triangles_0Triangles(Triangles* triangles);
Triangles* Triangles_Triangles(int count);


#endif
