#include <stdio.h>

#include "rcbc_xml_minixml_visualscene.h"
#include "rcbc_xml_minixml.h"
#include "console.h"

int RCBC_MiniXML_Init() {
	logit("Initilizing MiniXML...");
	return 0;
}

void DumpNodeInfo(mxml_node_t *node) {
	debugit(DEBUG_LOW, "------NODE_PTR: %p------", node);
	if(node == NULL) {
		debugit(DEBUG_LOW, "Null Node!");
		return;
	}

	switch(node->type) {
		case MXML_IGNORE: debugit(DEBUG_LOW, "TYPE: MXML_IGNORE"); break;
		case MXML_ELEMENT: debugit(DEBUG_LOW, "TYPE: MXML_ELEMENT"); break;
		case MXML_INTEGER: debugit(DEBUG_LOW, "TYPE: MXML_INTEGER"); break;
		case MXML_OPAQUE: debugit(DEBUG_LOW, "TYPE: MXML_OPAQUE"); break;
		case MXML_REAL: debugit(DEBUG_LOW, "TYPE: MXML_REAL"); break;
		case MXML_TEXT: debugit(DEBUG_LOW, "TYPE: MXML_TEXT"); break;
		case MXML_CUSTOM: debugit(DEBUG_LOW, "TYPE: MXML_CUSTOM"); break;
		dafault: debugit(DEBUG_LOW, "TYPE: BAD TYPE!!!", SYMBOL_WARNING);
	}

	switch(node->type) {
		case MXML_IGNORE: debugit(DEBUG_LOW, "NO DUMP INFO FOR THIS TYPE"); break;
		case MXML_ELEMENT: debugit(DEBUG_LOW, "NAME: %s", node->value.element.name); break;
		case MXML_INTEGER: debugit(DEBUG_LOW, "NO DUMP INFO FOR THIS TYPE"); break;
		case MXML_OPAQUE: debugit(DEBUG_LOW, "DUMP: %s", node->value.opaque); break;
		case MXML_REAL: debugit(DEBUG_LOW, "NO DUMP INFO FOR THIS TYPE"); break;
		case MXML_TEXT: debugit(DEBUG_LOW, "TEXT: '%s' WS=%d", node->value.text.string, node->value.text.whitespace); break;
		case MXML_CUSTOM: debugit(DEBUG_LOW, "NO DUMP INFO FOR THIS TYPE"); break;
	}
}

int RCBC_MiniXML_Load(RCBCThing* thing, char* filename) {
	logit("[MINIXML]: Opening '%s'...", filename);
	if(!thing) {
		errorit("[MINIXML]: Passed NULL 'thing'...");
	}

	FILE *fp;
	mxml_node_t *tree;
	fp = fopen(filename, "r");
	if(!fp) {
		errorit("[MINIXML]: Error opening %s... %s", filename, SYMBOL_FATAL);
		return 1;
	}
	logit("[MINIXML]: Parsing '%s'...", filename);
	tree = mxmlLoadFile(NULL, fp, MXML_OPAQUE_CALLBACK);
	fclose(fp);
	
	if(!tree) {
		errorit("[MINIXML]: Error parsing %s... %s", filename, SYMBOL_FATAL);
		return 1;
	}
	logit("[MINIXML]: Successfuly loaded... %s", SYMBOL_SMILEY);

	DumpNodeInfo(tree);

	mxml_node_t* node;

	node = mxmlFindElement(tree, tree, "library_geometries", NULL, NULL, MXML_DESCEND);
	RCBC_MiniXML_ProcessGeometries(thing, node);

	node = mxmlFindElement(tree, tree, "visual_scene", NULL, NULL, MXML_DESCEND);
	RCBC_MiniXML_ProcessVisualScene(thing, node);

	/* Free memory */
	//LLFree(hookups); /* TODO: <--- */
	//RCBC_HookupsFree(hookups);
	RCBC_Hookup_Execute(thing->sources, thing->sinks);
	RCBC_Hookup_Execute(thing->sources, thing->sinks);
	RCBC_Hookup_Execute(thing->sources, thing->sinks);
  mxmlDelete(tree);

	return 0;
}
