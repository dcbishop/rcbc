#include "rcbc.h"
#include "rcbc_data.h"
#include "console.h"

void RCBC_NodeDebugInfo(RCBCNode* node) {
	debugit(DEBUG_ALWAYS, "Node: Scale %f %f %f", node->scale[0], node->scale[1], node->scale[2]);
	debugit(DEBUG_ALWAYS, "      Translate %f %f %f", node->translate[0], node->translate[1], node->translate[2]);
}

RCBCThing* RCBC_ThingGenerate() {
	RCBCThing *thing = malloc(sizeof(RCBCThing));
	if(!thing) {
		return NULL;
	}
	thing->visual_scene = NULL;
	return thing;
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

	node->translate[0] = 0.0f;
	node->translate[1] = 0.0f;
	node->translate[2] = 0.0f;

	node->rotate = NULL;

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
	if(!node) {
		return;
	}

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

	mesh->next = NULL;
	mesh->prev = NULL;

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
