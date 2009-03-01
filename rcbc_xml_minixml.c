#include <stdio.h>

#include <mxml.h>

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

void RCRB_MiniXML_ProcessVisualScene_Node_Scale(RCBCNode *rnode, mxml_node_t *xnode) {
	if(!rnode) {
		errorit("XML Passed NULL RCBC node... %s", SYMBOL_WARNING);
		return;
	} 
	if(!xnode) {
		errorit("XML Passed NULL XML node... %s", SYMBOL_WARNING);
		return;
	}

	sscanf(xnode->value.opaque, "%f %f %f", &rnode->scale[0], &rnode->scale[1], &rnode->scale[2]);
}


void RCRB_MiniXML_ProcessVisualScene_Node_Translate(RCBCNode *rnode, mxml_node_t *xnode) {
	if(!rnode) {
		errorit("XML Passed NULL RCBC node... %s", SYMBOL_WARNING);
		return;
	} 
	if(!xnode) {
		errorit("XML Passed NULL XML node... %s", SYMBOL_WARNING);
		return;
	}

	sscanf(xnode->value.opaque, "%f %f %f", &rnode->translate[0], &rnode->translate[1], &rnode->translate[2]);
}

void RCRB_MiniXML_ProcessVisualScene_Node_Rotate(RCBCNode *rnode, mxml_node_t *xnode) {
	RCBCNode_Rotate *rotate;

	if(!rnode) {
		errorit("XML Passed NULL RCBC node... %s", SYMBOL_WARNING);
		return;
	} 
	if(!xnode) {
		errorit("XML Passed NULL XML node... %s", SYMBOL_WARNING);
		return;
	}

	if(rnode->rotate == NULL) {
		rotate = rnode->rotate = malloc(sizeof(RCBCNode_Rotate));
	} else {
		for(rotate = rnode->rotate; rotate->next; rotate = rotate->next) {}
		rotate->next = malloc(sizeof(RCBCNode_Rotate));	
	}

	rotate->x = 0.0f;
	rotate->y = 0.0f;
	rotate->z = 0.0f;
	rotate->angle = 0.0f;
	rotate->next = NULL;

	sscanf(xnode->value.opaque, "%f %f %f %f", &rotate->x, &rotate->y, &rotate->z, &rotate->angle);
}

void RCRB_MiniXML_ProcessVisualScene_Node(RCBCNode **rnode, mxml_node_t *xnode) {
	RCBCNode* last;

	if(!rnode) {
		errorit("XML Passed NULL RCBC node... %s", SYMBOL_WARNING);
		return;
	} 
	if(!xnode) {
		errorit("XML Passed NULL XML node... %s", SYMBOL_WARNING);
		return;
	}

	if(!(*rnode)) {
		*rnode = RCBC_NodeGenerate();
	} else {
		// Loop until end node
		for(last = *rnode; last->next; last = last->next) {}
		last->next = RCBC_NodeGenerate();
	}

	mxml_node_t *child;
	const char *id = mxmlElementGetAttr(xnode, "id");
	debugit(DEBUG_LOW, "NODE ID: '%s'", id);
	for(child = xnode->child; child != NULL; child = child->next) {
		if(child->type == MXML_ELEMENT) {
			DumpNodeInfo(child);
			if(strcasecmp(child->value.element.name, "node") == 0) {
				RCRB_MiniXML_ProcessVisualScene_Node(&((*rnode)->child), child);
			} else if(strcasecmp(child->value.element.name, "translate") == 0) {
				RCRB_MiniXML_ProcessVisualScene_Node_Translate(*rnode, child->child);
			} else if(strcasecmp(child->value.element.name, "rotate") == 0) {
				RCRB_MiniXML_ProcessVisualScene_Node_Rotate(*rnode, child);
			} else if(strcasecmp(child->value.element.name, "scale") == 0) {
				RCRB_MiniXML_ProcessVisualScene_Node_Scale(*rnode, child->child);
			} 
		}
	}
}

void RCRB_MiniXML_ProcessVisualScene(RCBCThing *thing, mxml_node_t *node) {
	const char* id;

	if(!thing) {
		errorit("XML Passed NULL 'thing'... %s", SYMBOL_WARNING);
		return;
	} 
	if(!node) {
		errorit("XML Passed NULL xml node... %s", SYMBOL_WARNING);
		return;
	}

	if(thing->visual_scene) {
		RCBC_NodeFree(&(thing->visual_scene));
	}

	for(node = node->child; node != NULL; node = node->next) {
		if(node->type == MXML_ELEMENT) {
			DumpNodeInfo(node);

			if(strcasecmp(node->value.element.name, "node") == 0) {				
				RCRB_MiniXML_ProcessVisualScene_Node(&thing->visual_scene, node);			
			}
		}
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

	node = mxmlFindElement(tree, tree, "visual_scene", NULL, NULL, MXML_DESCEND);
	RCRB_MiniXML_ProcessVisualScene(thing, node);

  mxmlDelete(tree);
	return 0;
}
