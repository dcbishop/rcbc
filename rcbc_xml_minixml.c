#include <stdio.h>

#include "rcbc_xml_minixml_visualscene.h"
#include "rcbc_xml_minixml_textures.h"
#include "rcbc_xml_minixml_geometries.h"
#include "rcbc_xml_minixml.h"
#include "console.h"

/** 
 * Initilize the MiniXML library (not actually required...)
 */
int RCBC_MiniXML_Init() {
	LOG("Initilizing MiniXML...");
	return 0;
}

/**
 * For debugging puroses, dump XML node information
 */
void DumpNodeInfo(mxml_node_t *node) {
	DEBUG(DEBUG_VERY_HIGH, "------NODE_PTR: %p------", node);
	if(node == NULL) {
		DEBUG(DEBUG_VERY_HIGH, "Null Node!");
		return;
	}

	switch(node->type) {
		case MXML_IGNORE: DEBUG(DEBUG_VERY_HIGH, "TYPE: MXML_IGNORE"); break;
		case MXML_ELEMENT: DEBUG(DEBUG_VERY_HIGH, "TYPE: MXML_ELEMENT"); break;
		case MXML_INTEGER: DEBUG(DEBUG_VERY_HIGH, "TYPE: MXML_INTEGER"); break;
		case MXML_OPAQUE: DEBUG(DEBUG_VERY_HIGH, "TYPE: MXML_OPAQUE"); break;
		case MXML_REAL: DEBUG(DEBUG_VERY_HIGH, "TYPE: MXML_REAL"); break;
		case MXML_TEXT: DEBUG(DEBUG_VERY_HIGH, "TYPE: MXML_TEXT"); break;
		case MXML_CUSTOM: DEBUG(DEBUG_VERY_HIGH, "TYPE: MXML_CUSTOM"); break;
		dafault: DEBUG(DEBUG_VERY_HIGH, "TYPE: BAD TYPE!!!", SYMBOL_WARNING);
	}

	switch(node->type) {
		case MXML_IGNORE: DEBUG(DEBUG_VERY_HIGH, "NO DUMP INFO FOR THIS TYPE"); break;
		case MXML_ELEMENT: DEBUG(DEBUG_VERY_HIGH, "NAME: %s", node->value.element.name); break;
		case MXML_INTEGER: DEBUG(DEBUG_VERY_HIGH, "NO DUMP INFO FOR THIS TYPE"); break;
		case MXML_OPAQUE: DEBUG(DEBUG_VERY_HIGH, "DUMP: %s", node->value.opaque); break;
		case MXML_REAL: DEBUG(DEBUG_VERY_HIGH, "NO DUMP INFO FOR THIS TYPE"); break;
		case MXML_TEXT: DEBUG(DEBUG_VERY_HIGH, "TEXT: '%s' WS=%d", node->value.text.string, node->value.text.whitespace); break;
		case MXML_CUSTOM: DEBUG(DEBUG_VERY_HIGH, "NO DUMP INFO FOR THIS TYPE"); break;
	}
}

/**
 * Load a model from a COLLADA file
 */
int RCBC_MiniXML_Load(Model* model, List* images, char* filename) {
	LOG("[MINIXML]: Opening '%s'...", filename);
	if(!model) {
		ERROR("[MINIXML]: Passed NULL 'model'...");
	}

	FILE *fp;
	mxml_node_t *tree;
	fp = fopen(filename, "r");
	if(!fp) {
		ERROR("[MINIXML]: Error opening %s... %s", filename, SYMBOL_FATAL);
		return 1;
	}
	LOG("[MINIXML]: Parsing '%s'...", filename);
	tree = mxmlLoadFile(NULL, fp, MXML_OPAQUE_CALLBACK);
	fclose(fp);
	
	if(!tree) {
		ERROR("[MINIXML]: Error parsing %s... %s", filename, SYMBOL_FATAL);
		return 1;
	}
	LOG("[MINIXML]: Successfuly loaded... %s", SYMBOL_SMILEY);

	DumpNodeInfo(tree);

	ModelTempory* tempory = NEW(ModelTempory);
	tempory->model = model;
	tempory->images = images;
	mxml_node_t* node;

	node = mxmlFindElement(tree, tree, "library_geometries", NULL, NULL, MXML_DESCEND);
	RCBC_MiniXML_ProcessGeometries(tempory, node);

	node = mxmlFindElement(tree, tree, "visual_scene", NULL, NULL, MXML_DESCEND);
	RCBC_MiniXML_ProcessVisualScene(tempory, node);

	node = mxmlFindElement(tree, tree, "library_images", NULL, NULL, MXML_DESCEND);
	RCBC_MiniXML_ProcessTextureImages(tempory, node);

	node = mxmlFindElement(tree, tree, "library_materials", NULL, NULL, MXML_DESCEND);
	RCBC_MiniXML_ProcessTextureMaterial(tempory, node);

	node = mxmlFindElement(tree, tree, "library_effects", NULL, NULL, MXML_DESCEND);
	RCBC_MiniXML_ProcessTextureEffects(tempory, node);

	Hookup_Debug(tempory->sources);
	Hookup_Debug(tempory->sinks);

	Hookup_Execute(tempory->sources, tempory->sinks);
	Hookup_Execute(tempory->sources, tempory->sinks);
	Hookup_Execute(tempory->sources, tempory->sinks);

	DEBUG(DEBUG_LOW, "[MINIXML]: Hookups processed");

	ListNode* itr = tempory->unsorted->first;
	while(itr) {
		RCBC_SortTriangles(itr->data);
		itr = itr->next;
	}

	#warning TODO: Consider removing nodes with no geometry or children (camera/light nodes.)

	#warning TODO: Free memory, cleanup segfaults
	/* Free memory */
	/*HookupFree(model->sources);
	HookupFree(model->sinks);*/
	//HookupsFree(hookups);
	//ListFree(hookups);
	//Free tempory

  mxmlDelete(tree);
	return 0;
}
