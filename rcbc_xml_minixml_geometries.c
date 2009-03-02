#include "rcbc_xml_minixml_geometries.h"
#include "rcbc_data.h"
#include "console.h"

int RCRB_MiniXML_ProcessGeometries_Mesh(RCBCMesh **mesh, mxml_node_t *xnode) {
	RCBCMesh* last;

	if(!mesh) {
		errorit("XML Passed NULL RCBC mesh... %s", SYMBOL_WARNING);
		return;
	} 
	if(!xnode) {
		errorit("XML Passed NULL XML node... %s", SYMBOL_WARNING);
		return;
	}

	warnit("Info");
	if(!(*mesh)) {
		debugit(DEBUG_LOW, "NULL MESH");				
		*mesh = RCBC_MeshGenerate();
		last = *mesh;
	} else {
		// Loop until end node
		debugit(DEBUG_LOW, "NON-NULL MESH");
		for(last = *mesh; last->next; last = last->next) {	debugit(DEBUG_LOW, "\twalking..."); }
		last->next = RCBC_MeshGenerate();
		last->next->prev = last;
		last = last->next;
	}

	mxml_node_t *child;
	const char *id = mxmlElementGetAttr(xnode, "id");
	debugit(DEBUG_LOW, "MESH ID: '%s'", id);
	for(child = xnode->child; child != NULL; child = child->next) {
		if(child->type == MXML_ELEMENT) {
			//RCRB_MiniXML_ProcessGeometries_Mesh_Children(last, child);
		}
	}

	RCBC_NodeDebugInfo(last);
}


/* Grabs the mesh from the geometry (COLLADA specs say there can be only one  
 * although a few nonmesh things are supported, they are usless to us and ignored) */ 
int RCRB_MiniXML_ProcessGeometries_Geometry(RCBCThing *thing, mxml_node_t *node) {
	if(!thing) {
		errorit("XML Passed NULL 'thing'... %s", SYMBOL_WARNING);
		return;
	} 
	if(!node) {
		errorit("XML Passed NULL xml node... %s", SYMBOL_WARNING);
		return;
	}

	mxml_node_t* mesh_node = mxmlFindElement(node, node, "mesh", NULL, NULL, MXML_DESCEND);
	if(!mesh_node) {
		errorit("No mesh node in geometry!");
		return 1;
	}

	return 0;
}

int RCRB_MiniXML_ProcessGeometries(RCBCThing *thing, mxml_node_t *node) {
	const char* id;

	if(!thing) {
		errorit("XML Passed NULL 'thing'... %s", SYMBOL_WARNING);
		return 1;
	} 
	if(!node) {
		errorit("XML Passed NULL xml node... %s", SYMBOL_WARNING);
		return 1;
	}

	if(thing->geometries) {
		RCBC_MeshFree(&(thing->geometries));
	}

	for(node = node->child; node != NULL; node = node->next) {
		if(node->type == MXML_ELEMENT) {
			DumpNodeInfo(node);

			if(strcasecmp(node->value.element.name, "geometry") == 0) {
				return RCRB_MiniXML_ProcessGeometries_Geometry(&thing->geometries, node);			
			}
		}
	}
}
