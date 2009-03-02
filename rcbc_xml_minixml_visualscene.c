#include "rcbc_xml_minixml_visualscene.h"
#include "console.h"

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
		rotate = rotate->next;
	}

	rotate->x = 0.0f;
	rotate->y = 0.0f;
	rotate->z = 0.0f;
	rotate->angle = 0.0f;
	rotate->next = NULL;

	sscanf(xnode->value.opaque, "%f %f %f %f", &rotate->x, &rotate->y, &rotate->z, &rotate->angle);
}

void RCRB_MiniXML_ProcessVisualScene_Node_Children(RCBCNode *rnode, mxml_node_t *xnode) {
			DumpNodeInfo(xnode);
			if(strcasecmp(xnode->value.element.name, "node") == 0) {
				RCRB_MiniXML_ProcessVisualScene_Node(&(rnode->child), xnode);
			} else if(strcasecmp(xnode->value.element.name, "translate") == 0) {
				RCRB_MiniXML_ProcessVisualScene_Node_Translate(rnode, xnode->child);
			} else if(strcasecmp(xnode->value.element.name, "rotate") == 0) {
				RCRB_MiniXML_ProcessVisualScene_Node_Rotate(rnode, xnode);
			} else if(strcasecmp(xnode->value.element.name, "scale") == 0) {
				RCRB_MiniXML_ProcessVisualScene_Node_Scale(rnode, xnode->child);
			} 
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

	warnit("Info");
	if(!(*rnode)) {
		debugit(DEBUG_LOW, "NULL NODE");				
		*rnode = RCBC_NodeGenerate();
		last = *rnode;
	} else {
		// Loop until end node
		debugit(DEBUG_LOW, "NON-NULL NODE");
		for(last = *rnode; last->next; last = last->next) {	debugit(DEBUG_LOW, "\twalking..."); }
		last->next = RCBC_NodeGenerate();
		last->next->prev = last;
		last = last->next;
	}

	mxml_node_t *child;
	const char *id = mxmlElementGetAttr(xnode, "id");
	debugit(DEBUG_LOW, "NODE ID: '%s'", id);
	for(child = xnode->child; child != NULL; child = child->next) {
		if(child->type == MXML_ELEMENT) {
			RCRB_MiniXML_ProcessVisualScene_Node_Children(last, child);
		}
	}

	RCBC_NodeDebugInfo(last);
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
