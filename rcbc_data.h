/* Contains COLLADA data scructures */
#ifndef _RCBC_DATA_DEF
#define _RCBC_DATA_DEF

/* A container for a COLLADA model */
typedef struct RCBCThing {
	struct RCBCNode* visual_scene;
	struct RCBCmesh* geometries;
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

typedef struct RCBCMesh {
	struct RCBCMesh *prev;	
	struct RCBCMesh *next;
} RCBCMesh;

RCBCThing* RCBC_ThingGenerate();

void RCBC_NodeDebugInfo(RCBCNode* node);
RCBCNode* RCBC_NodeGenerate();
void RCBC_NodeFree(RCBCNode **node);

#endif
