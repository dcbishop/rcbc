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
	sscanf(xnode->value.opaque, "%f %f %f", &rnode->translate[0], &rnode->translate[2], &rnode->translate[1]);
	rnode->translate[0] = -rnode->translate[0];
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

	sscanf(xnode->value.opaque, "%f %f %f %f", &rotate->x, &rotate->z, &rotate->y, &rotate->angle);
	rotate->x = -rotate->x;
	DumpNodeInfo(xnode);

	LLAdd(&rnode->rotations, rotate);
	return rotate;
}

void RCBC_MiniXML_ProcessVisualScene_Node_InstanceGeometry(RCBC_Tempory *tempory, RCBCNode *rnode, mxml_node_t *xnode) {
	mxml_node_t *child1, *child2, *child3, *child4;
	const char *semantic, *input_semantic;
	const char *id = mxmlElementGetAttr(xnode, "url");
	if(id[0] == '#') {
		id++;
	}

	RCBC_Hookup* hookup = RCBC_HookupGenerate((char*)id, (void*)&rnode->mesh);
	LLAdd(&tempory->sinks, hookup);

	/* TODO: Too many loops (although most only fire once... ) */
	/* TODO: This is probably not even needed here... */
	/*for(child1 = xnode->child; child1 != NULL; child1 = child1->next) {
		if(child1->type == MXML_ELEMENT &&
			(strcasecmp(child1->value.element.name, "bind_material") == 0)) {

			for(child2 = child1->child; child2 != NULL; child2 = child2->next) {
				if(child2->type == MXML_ELEMENT &&
					(strcasecmp(child2->value.element.name, "technique_common") == 0)) {
	
					for(child3 = child2->child; child3 != NULL; child3 = child3->next) {
						if(child3->type == MXML_ELEMENT &&
						(strcasecmp(child3->value.element.name, "instance_material") == 0)) {

							for(child4 = child3->child; child4 != NULL; child4 = child4->next) {
								if(child4->type == MXML_ELEMENT &&
								(strcasecmp(child4->value.element.name, "bind_vertex_input") == 0)) {
									input_semantic = mxmlElementGetAttr(xnode, "input_semantic");									
									semantic = mxmlElementGetAttr(xnode, "semantic");
									hookup = RCBC_HookupGenerate((char*)semantic, (void*)&rnode->mesh);
								}
							}


						}
					}


				}
			}


		}
	}*/
			
}

void RCBC_MiniXML_ProcessVisualScene_Node_Children(RCBC_Tempory *tempory, RCBCNode *rnode, mxml_node_t *xnode) {
	assert(rnode);
	assert(xnode);
	DumpNodeInfo(xnode);
	if(strcasecmp(xnode->value.element.name, "node") == 0) {
		RCBC_MiniXML_ProcessVisualScene_Node(tempory, &(rnode->child), xnode);
	} else if(strcasecmp(xnode->value.element.name, "translate") == 0) {
		RCBC_MiniXML_ProcessVisualScene_Node_Translate(rnode, xnode->child);
	} else if(strcasecmp(xnode->value.element.name, "rotate") == 0) {
		RCBC_MiniXML_ProcessVisualScene_Node_Rotate(rnode, xnode->child);
	} else if(strcasecmp(xnode->value.element.name, "scale") == 0) {
		RCBC_MiniXML_ProcessVisualScene_Node_Scale(rnode, xnode->child);
	} else if(strcasecmp(xnode->value.element.name, "instance_geometry") == 0) {
		RCBC_MiniXML_ProcessVisualScene_Node_InstanceGeometry(tempory, rnode, xnode);
	}
}

void RCBC_MiniXML_ProcessVisualScene_Node(RCBC_Tempory *tempory, RCBCNode **rnode, mxml_node_t *xnode) {
	RCBCNode* last;

	assert(tempory);
	assert(rnode);
	assert(xnode);

	if(!(*rnode)) {
		*rnode = RCBC_NodeGenerate();
		last = *rnode;
	} else {
		// Loop until end node
		for(last = *rnode; last->next; last = last->next) {}
		last->next = RCBC_NodeGenerate();
		last->next->prev = last;
		last = last->next;
	}

	mxml_node_t *child;
	const char *id = mxmlElementGetAttr(xnode, "id");

	DEBUG(DEBUG_HIGH, "Processing NODE ID: '%s'", id);
	for(child = xnode->child; child != NULL; child = child->next) {
		if(child->type == MXML_ELEMENT) {
			RCBC_MiniXML_ProcessVisualScene_Node_Children(tempory, last, child);
		}
	}

	RCBC_NodeDebugInfo(last);
}

void RCBC_MiniXML_ProcessVisualScene(RCBC_Tempory *tempory, mxml_node_t *node) {

	assert(tempory);
	assert(node);

	if(tempory->thing->visual_scene) {
		RCBC_NodeFree(&(tempory->thing->visual_scene));
	}

	for(node = node->child; node != NULL; node = node->next) {
		if(node->type == MXML_ELEMENT) {
			DumpNodeInfo(node);

			if(strcasecmp(node->value.element.name, "node") == 0) {				
				RCBC_MiniXML_ProcessVisualScene_Node(tempory, &tempory->thing->visual_scene, node);			
			}
		}
	}
}
