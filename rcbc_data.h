/* Contains COLLADA data scructures */
#ifndef _RCBC_DATA_DEF
#define _RCBC_DATA_DEF

/* A generic linked list node */
typedef struct LLNode {
	void* data;
	struct LLNode* next;
} LLNode;

/* A generic linked list node */
typedef struct LL {
	int count;
	struct LLNode* head;
	struct LLNode* last;
} LL;


/* A container for a COLLADA model */
typedef struct RCBC_Model {
	struct RCBCNode* visual_scene;
	struct LLNode* geometries;
} RCBC_Model;

typedef struct RCBC_Tempory {
	RCBC_Model* thing;
	LLNode* sources;
	LLNode* sinks;
	LLNode* unsorted;
	LLNode* images;
} RCBC_Tempory;

/* Nodes can have multiple rotations */
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

typedef struct RCBC_Image {
	int id;
	char* filename;
} RCBC_Image;

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

/* Contains mesh data */
typedef struct RCBCMesh {
	LLNode* arrays;
	RCBC_Triangles* triangles;
} RCBCMesh;

/* A basic COLLADA node */
typedef struct RCBCNode {
	RCBCMesh* mesh;

	float translate[3];

	struct LLNode* rotations;

	float scale[3];

	struct RCBCNode* next;
	struct RCBCNode* prev;
	struct RCBCNode* child;
	struct RCBCNode* parent;
} RCBCNode;

/* This is used to link between XML named id's and the actual data pointers */
typedef struct RCBC_Hookup {
	int type; /* Is this an id looking for a pointer, or a pointer looking for an id */
	char* id; /* The XML id string */
	void** ptr;
} RCBC_Hookup;

LLNode* LLGenerate(void* data);
LLNode* LLAdd(LLNode** rootnode, void* data);
void LLFree(LLNode** rootnode);

RCBC_Model* RCBC_ThingGenerate();

void RCBC_NodeDebugInfo(RCBCNode* node);
RCBCNode* RCBC_NodeGenerate();
void RCBC_NodeFree(RCBCNode **node);

RCBCMesh* RCBC_MeshGenerate();
void RCBC_MeshFree(RCBCMesh **mesh);

RCBC_FloatArray* RCBC_FloatArrayGenerate(int count);

RCBC_Hookup* RCBC_HookupGenerate(char* id, void* pointer);
RCBC_Hookup* RCBC_HookupFind(LLNode* roothookup, char* id);

RCBC_TrianglesUnsorted* RCBC_TrianglesUnsortedGenerate();
RCBC_Tempory* RCBC_TemporyGenerate();

RCBC_Image* RCBC_ImageGenerate(char* filename);
#endif
