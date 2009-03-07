#include <assert.h>
#include "rcbc_xml_minixml_visualscene.h"
#include "console.h"

void RCBC_MiniXML_ProcessVisualScene_Node_Scale(RCBCNode *rnode, mxml_node_t *xnode) {
	assert(rnode);
	assert(xnode);

	sscanf(xnode->value.opaque, "%f %f %f", &rnode->scale[0], &rnode->scale[1], &rnode->scale[2]);
}


void RCBC_MiniXML_ProcessVisualScene_Node_Translate(RCBCNode *rnode, mxml_node_t *xnode) {
	assert(rnode);
	assert(xnode);
	sscanf(xnode->value.opaque, "%f %f %f", &rnode->translate[0], &rnode->translate[1], &rnode->translate[2]);
}

RCBCNode_Rotate* RCBC_MiniXML_ProcessVisualScene_Node_Rotate(RCBCNode *rnode, mxml_node_t *xnode) {
	RCBCNode_Rotate *rotate;

	assert(rnode);
	assert(xnode);

	rotate = malloc(sizeof(RCBCNode_Rotate));
	if(!rotate) {
		return NULL;
	}

	rotate->x = 0.0f;
	rotate->y = 0.0f;
	rotate->z = 0.0f;
	rotate->angle = 0.0f;

	sscanf(xnode->value.opaque, "%f %f %f %f", &rotate->x, &rotate->y, &rotate->z, &rotate->angle);
	debugit(DEBUG_LOW, "%sROTATE: %s", COLOUR_LIGHT_MAGENTA, xnode->value.opaque);
	DumpNodeInfo(xnode);
debugit(DEBUG_LOW, "%s------------- %f %f %f %f", COLOUR_LIGHT_MAGENTA, rotate->x, rotate->y, rotate->z, rotate->angle);	
	LLAdd(&rnode->rotations, rotate);
	return rotate;
}

void RCBC_MiniXML_ProcessVisualScene_Node_Children(RCBCThing *thing, RCBCNode *rnode, mxml_node_t *xnode) {
	assert(rnode);
	assert(xnode);
	DumpNodeInfo(xnode);
	if(strcasecmp(xnode->value.element.name, "node") == 0) {
		RCBC_MiniXML_ProcessVisualScene_Node(thing, &(rnode->child), xnode);
	} else if(strcasecmp(xnode->value.element.name, "translate") == 0) {
		RCBC_MiniXML_ProcessVisualScene_Node_Translate(rnode, xnode->child);
	} else if(strcasecmp(xnode->value.element.name, "rotate") == 0) {
		RCBC_MiniXML_ProcessVisualScene_Node_Rotate(rnode, xnode->child);
	} else if(strcasecmp(xnode->value.element.name, "scale") == 0) {
		RCBC_MiniXML_ProcessVisualScene_Node_Scale(rnode, xnode->child);
	} else if(strcasecmp(xnode->value.element.name, "instance_geometry") == 0) {
			const char *id = mxmlElementGetAttr(xnode, "url");
			if(id[0] == '#') {
				id++;
			}
			RCBC_Hookup* hookup = RCBC_HookupGenerate((char*)id, (void*)&rnode->mesh);
			LLAdd(&thing->sinks, hookup);
	}
}

void RCBC_MiniXML_ProcessVisualScene_Node(RCBCThing *thing, RCBCNode **rnode, mxml_node_t *xnode) {
	RCBCNode* last;

	assert(rnode);
	assert(xnode);

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

	RCBC_Hookup* hookup = RCBC_HookupGenerate((char*)id, (void*)last);

	debugit(DEBUG_LOW, "NODE ID: '%s'", id);
	for(child = xnode->child; child != NULL; child = child->next) {
		if(child->type == MXML_ELEMENT) {
			RCBC_MiniXML_ProcessVisualScene_Node_Children(thing, last, child);
		}
	}
	LLAdd(&thing->sinks, hookup);
	RCBC_NodeDebugInfo(last);
}

void RCBC_MiniXML_ProcessVisualScene(RCBCThing *thing, mxml_node_t *node) {

	assert(thing);
	assert(node);

	if(thing->visual_scene) {
		RCBC_NodeFree(&(thing->visual_scene));
	}

	for(node = node->child; node != NULL; node = node->next) {
		if(node->type == MXML_ELEMENT) {
			DumpNodeInfo(node);

			if(strcasecmp(node->value.element.name, "node") == 0) {				
				RCBC_MiniXML_ProcessVisualScene_Node(thing, &thing->visual_scene, node);			
			}
		}
	}
}
