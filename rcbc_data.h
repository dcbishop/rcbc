/* Contains COLLADA data scructures */
#ifndef _RCBC_DATA_DEF
#define _RCBC_DATA_DEF

/* A container for a COLLADA model */
typedef struct RCBCThing {
	struct RCBCNode* visual_scene;
	struct LLNode* geometries;
} RCBCThing;

/* A basic COLLADA node */
typedef struct RCBCNode {
	float translate[3];

	struct RCBCNode_Rotate* rotate;

	float scale[3];

	struct RCBCNode* next;
	struct RCBCNode* prev;
	struct RCBCNode* child;
	struct RCBCNode* parent;
} RCBCNode;

/* Nodes can have multiple rotations */
typedef struct RCBCNode_Rotate {
	float x;
	float y;
	float z;
	float angle;
	struct RCBCNode_Rotate* next;
} RCBCNode_Rotate;

/* A generic linked list node */
typedef struct LLNode {
	void* data;
	struct LLNode* next;
} LLNode;

typedef struct RCBC_FloatArray {
	int count;
	float* values;
} RCBC_FloatArray;

/* Contains mesh data */
typedef struct RCBCMesh {
	RCBC_FloatArray* vertices;
	RCBC_FloatArray* normals;
	RCBC_FloatArray* textures;

	LLNode* arrays;

	LLNode* id2ptr;
	LLNode* ptr2id;
} RCBCMesh;

#define HOOKUP_PTR2ID 0
#define HOOKUP_ID2PTR 1

/* This is used to link between XML named id's and the actual data pointers */
typedef struct RCBC_Hookup {
	int type; /* Is this an id looking for a pointer, or a pointer looking for an id */
	char* id; /* The XML id string */
	void** ptr;
} RCBC_Hookup;

LLNode* LLGenerate(void* data);
LLNode* LLAdd(LLNode** rootnode, void* data);
void LLFree(LLNode** rootnode);

RCBCThing* RCBC_ThingGenerate();

void RCBC_NodeDebugInfo(RCBCNode* node);
RCBCNode* RCBC_NodeGenerate();
void RCBC_NodeFree(RCBCNode **node);

RCBCMesh* RCBC_MeshGenerate();
void RCBC_MeshFree(RCBCMesh **mesh);

RCBC_FloatArray* RCBC_FloatArrayGenerate(int count);

RCBC_Hookup* RCBC_HookupGenerate(char* id, void** pointer);
RCBC_Hookup* RCBC_HookupFind(LLNode* roothookup, char* id);
//TemporyHookup* TemporyHookup_Add(RCBC_Hookup** roothookup, int type, char* id, void* pointer);
//void TemporyHookup_Free(RCBC_Hookup* hookup);

#endif
