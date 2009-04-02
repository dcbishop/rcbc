#include <assert.h>
#include "rcbc_xml_minixml.h"
#include "rcbc_xml_minixml_visualscene.h"
#include "console.h"

void RCBC_MiniXML_ProcessVisualScene_Node_Scale(ModelTempory *tempory, SceneNode *rnode, mxml_node_t *xnode) {
	assert(rnode);
	assert(xnode);

	sscanf(xnode->value.opaque, "%f %f %f", &rnode->scale[0], &rnode->scale[1], &rnode->scale[2]);
	RCBC_FixAxis(tempory->up_axis, &rnode->scale[0], &rnode->scale[1], &rnode->scale[2]);
}

void RCBC_MiniXML_ProcessVisualScene_Node_Translate(ModelTempory *tempory, SceneNode *rnode, mxml_node_t *xnode) {
	assert(rnode);
	assert(xnode);

	sscanf(xnode->value.opaque, "%f %f %f", &rnode->translate[0], &rnode->translate[1], &rnode->translate[2]);
	RCBC_FixAxis(tempory->up_axis, &rnode->translate[0], &rnode->translate[1], &rnode->translate[2]);
}

Rotate* RCBC_MiniXML_ProcessVisualScene_Node_Rotate(ModelTempory *tempory, SceneNode *rnode, mxml_node_t *xnode) {
	GLfloat tmp;
	Rotate *rotate = NEW(Rotate);

	assert(rnode);
	assert(xnode);
	assert(rotate);

	sscanf(xnode->value.opaque, "%f %f %f %f", &rotate->x, &rotate->y, &rotate->z, &rotate->angle);
	RCBC_FixAxis(tempory->up_axis, &rotate->x, &rotate->y, &rotate->z);

	DumpNodeInfo(xnode);

	ListAdd(rnode->rotations, rotate);
	return rotate;
}

void RCBC_MiniXML_ProcessVisualScene_Node_InstanceGeometry(ModelTempory *tempory, SceneNode *rnode, mxml_node_t *xnode) {
	mxml_node_t *bm_node, *tc_node, *im_node;
	const char *semantic, *input_semantic;
	const char *id = mxmlElementGetAttr(xnode, "url");
	if(id[0] == '#') {
		id++;
	}

	Hookup* hookup = NEW(Hookup, (char*)id, (void*)&rnode->mesh);
	ListAdd(tempory->sinks, hookup);
	
	bm_node = mxmlFindElement(xnode, xnode, "bind_material", NULL, NULL, MXML_DESCEND);
	if(!bm_node) {
		return;
	}
	
	tc_node = mxmlFindElement(bm_node, bm_node, "technique_common", NULL, NULL, MXML_DESCEND);
	if(!tc_node) {
		return;
	}
	
	im_node = mxmlFindElement(tc_node, tc_node, "instance_material", NULL, NULL, MXML_DESCEND);
	if(!im_node) {
		return;
	}
	
	const char* symbol = mxmlElementGetAttr(im_node, "symbol");
	const char* target = mxmlElementGetAttr(im_node, "target");
	DEHASH(target);
	
	if(!symbol || !target) {
		return;
	}
	
	Hookup* ims_hookup = NEW(Hookup, (char*)symbol, NULL);
	if(!ims_hookup) {
		return;
	}
	
	ListAdd(tempory->sources, ims_hookup);
	
	Hookup* imt_hookup = NEW(Hookup, (char*)target, &ims_hookup->ptr);
	ListAdd(tempory->sinks, imt_hookup);
}

void RCBC_MiniXML_ProcessVisualScene_Node_Children(ModelTempory *tempory, SceneNode *rnode, mxml_node_t *xnode) {
	assert(rnode);
	assert(xnode);
	DumpNodeInfo(xnode);
	if(strcasecmp(xnode->value.element.name, "node") == 0) {
		RCBC_MiniXML_ProcessVisualScene_Node(tempory, &(rnode->child), xnode);
	} else if(strcasecmp(xnode->value.element.name, "translate") == 0) {
		RCBC_MiniXML_ProcessVisualScene_Node_Translate(tempory, rnode, xnode->child);
	} else if(strcasecmp(xnode->value.element.name, "rotate") == 0) {
		RCBC_MiniXML_ProcessVisualScene_Node_Rotate(tempory, rnode, xnode->child);
	} else if(strcasecmp(xnode->value.element.name, "scale") == 0) {
		RCBC_MiniXML_ProcessVisualScene_Node_Scale(tempory, rnode, xnode->child);
	} else if(strcasecmp(xnode->value.element.name, "instance_geometry") == 0) {
		RCBC_MiniXML_ProcessVisualScene_Node_InstanceGeometry(tempory, rnode, xnode);
	}
}

void RCBC_MiniXML_ProcessVisualScene_Node(ModelTempory *tempory, SceneNode **rnode, mxml_node_t *xnode) {
	SceneNode* last;

	assert(tempory);
	assert(rnode);
	assert(xnode);

	#warning ['TODO']: This loops through the nodes, keep a last node refrence.
	
	if(!(*rnode)) {
		*rnode = NEW(SceneNode);
		last = *rnode;
	} else {
		// Loop until end node
		for(last = *rnode; last->next; last = last->next) {}
		last->next = NEW(SceneNode);
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

	SceneNodeDebugInfo(last);
}

void RCBC_MiniXML_ProcessVisualScene(ModelTempory *tempory, mxml_node_t *node) {
	assert(tempory);
	if(!node) {
		return;
	}

	if(tempory->model->visual_scene) {
		DELETE(&(tempory->model->visual_scene));
	}

	for(node = node->child; node != NULL; node = node->next) {
		if(node->type == MXML_ELEMENT) {
			DumpNodeInfo(node);

			if(strcasecmp(node->value.element.name, "node") == 0) {				
				RCBC_MiniXML_ProcessVisualScene_Node(tempory, &tempory->model->visual_scene, node);			
			}
		}
	}
}
