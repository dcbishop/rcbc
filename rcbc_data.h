/* Contains COLLADA data scructures */
#ifndef _RCBC_DATA_DEF
#define _RCBC_DATA_DEF

#include "ll.h"
#include "rcbc_data_hookup.h"

/**
 * A container for a COLLADA model.
 */
typedef struct RCBC_Model {
	struct RCBCNode* visual_scene;
	struct LL* geometries;
} RCBC_Model;

/**
 * A throwaway struct for holding tempory values while loading a model.
 */
typedef struct RCBC_Tempory {
	RCBC_Model* model;
	LL* sources;
	LL* sinks;
	LL* unsorted;
	LL* images;
} RCBC_Tempory;

/**
 *  Contains rotation information for a COLLADA scene node.
 **/
typedef struct RCBCNode_Rotate {
	float x;
	float y;
	float z;
	float angle;
} RCBCNode_Rotate;

typedef struct RCBC_FloatArray {
	int count;
	float* values;
} RCBC_FloatArray;

/** 
 * Contains a filename for a image, an id for opengl texture binding 
 * and the number of refrences to this image.
 */
typedef struct RCBC_Image {
	int id;
	char* filename;
	int refs;
} RCBC_Image;

/**
 * Contains unprocessed triangle data as it is read from the COLLADA
 * file.
 */
typedef struct RCBC_TrianglesUnsorted {
	void** ptr;
	unsigned int count;
	unsigned int inputs;
	unsigned int* indices;
	int vertices_offset;
	RCBC_FloatArray* vertices;
	int normals_offset;
	RCBC_FloatArray* normals;
	int texcoords_offset;
	RCBC_FloatArray* texcoords;
	RCBC_Image* image;
} RCBC_TrianglesUnsorted;

typedef struct RCBC_Triangles {
	unsigned int count;
	RCBC_FloatArray* vertices;
	RCBC_FloatArray* normals;
	RCBC_FloatArray* texcoords;
	RCBC_Image* image;
} RCBC_Triangles;

typedef struct Vector {
	float x, y, z;
} Vector;

/** 
 * Contains mesh data
 **/
typedef struct RCBCMesh {
	LL* arrays;
	RCBC_Triangles* triangles;
} RCBCMesh;

/**
 * A basic COLLADA scene node
 */
typedef struct RCBCNode {
	RCBCMesh* mesh;

	float translate[3];

	struct LL* rotations;

	float scale[3];

	struct RCBCNode* next;
	struct RCBCNode* prev;
	struct RCBCNode* child;
	struct RCBCNode* parent;
} RCBCNode;

LL* LLGenerate();
LLNode* LLAdd(LL* rootnode, void* data);
void LLFree(LL* rootnode);

RCBC_Model* RCBC_modelGenerate();

void RCBC_NodeDebugInfo(RCBCNode* node);
RCBCNode* RCBC_NodeGenerate();
void RCBC_NodeFree(RCBCNode **node);

RCBCMesh* RCBC_MeshGenerate();
void RCBC_MeshFree(RCBCMesh **mesh);

RCBC_FloatArray* RCBC_FloatArrayGenerate(int count);

RCBC_TrianglesUnsorted* RCBC_TrianglesUnsortedGenerate();
RCBC_Tempory* RCBC_TemporyGenerate();

RCBC_Image* RCBC_ImageGenerate(char* filename);

#endif
