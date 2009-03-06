#include <assert.h>

#include "rcbc_xml_minixml_geometries.h"
#include "rcbc_data.h"
#include "console.h"

/* Reads a float array, such as verticies, normals, text cords, etc... */
int RCBC_MiniXML_ProcessGeometries_Mesh_FloatArray(RCBCMesh *mesh, mxml_node_t *xnode) {
	const char* id = mxmlElementGetAttr(xnode, "id");
	const char* count_s = mxmlElementGetAttr(xnode, "count");
	int count = atoi(count_s);
	
	RCBC_FloatArray* newarray = RCBC_FloatArrayGenerate(count);

	float f = 0.0f;
	int i = 0;
	char* pch = strtok(xnode->child->value.opaque, " ");
	while(pch && i < count) {
		sscanf(pch, "%f", &f);
		newarray->values[i] = f;
		pch = strtok(NULL, " ");
		i++;
	}

	LLAdd(&mesh->arrays, newarray);
	RCBC_Hookup* hookup; 
	hookup = RCBC_HookupGenerate((char*)id, (void*)newarray);
	LLAdd(&mesh->id2ptr, hookup);

	return 0;
}

int RCBC_MiniXML_ProcessGeometries_Mesh_Source(RCBCMesh *mesh, mxml_node_t *xnode) {
	debugit(DEBUG_LOW, "%sRCBC_MiniXML_ProcessGeometries_Mesh_Source", COLOUR_LIGHT_BLUE);
	for(xnode = xnode; xnode; xnode = xnode->next) {
		if(xnode->type == MXML_ELEMENT) {
			DumpNodeInfo(xnode);

			if(strcasecmp(xnode->value.element.name, "float_array") == 0) {
				return RCBC_MiniXML_ProcessGeometries_Mesh_FloatArray(mesh, xnode);			
			}
		}
	}
	errorit("Failed to find float array...");
	return 1;
}

int RCBC_MiniXML_ProcessGeometries_Mesh_Children(RCBCMesh *mesh, mxml_node_t *xnode) {
	if(!mesh) {
		errorit("XML Passed NULL RCBC mesh... %s", SYMBOL_WARNING);
		return 1;
	} 
	if(!xnode) {
		errorit("XML Passed NULL XML node... %s", SYMBOL_WARNING);
		return 1;
	}
	DumpNodeInfo(xnode);
	if(strcasecmp(xnode->value.element.name, "source") == 0) {
		return RCBC_MiniXML_ProcessGeometries_Mesh_Source(mesh, xnode->child);
	} else if(strcasecmp(xnode->value.element.name, "verticies") == 0) {
		//return RCBC_MiniXML_ProcessGeometries_Mesh_Verticies(mesh, xnode, hookups);
	} else if(strcasecmp(xnode->value.element.name, "polygons") == 0) {
		//return RCBC_MiniXML_ProcessGeometries_Mesh_Polygons(mesh, xnode->child, hookups);
	} 
}

int RCBC_MiniXML_ProcessGeometries_Mesh(RCBCThing *thing, mxml_node_t *xnode) {
	RCBCMesh* last;

	assert(thing);
	assert(xnode);

	warnit("Info");

	/*
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
*/

	RCBCMesh* mesh = RCBC_MeshGenerate();
	LLAdd(&thing->geometries, mesh);

	mxml_node_t *child;
	const char *id = mxmlElementGetAttr(xnode, "id");
	debugit(DEBUG_LOW, "MESH ID: '%s'", id);
	for(child = xnode->child; child != NULL; child = child->next) {
		if(child->type == MXML_ELEMENT) {
			RCBC_MiniXML_ProcessGeometries_Mesh_Children(mesh, child);
		}
	}
	return 0;
}


/* Grabs the mesh from the geometry (COLLADA specs say there can be only one  
 * although a few nonmesh things are supported, they are usless to us and ignored) */ 
int RCBC_MiniXML_ProcessGeometries_Geometry(RCBCThing *thing, mxml_node_t *node) {
	assert(thing);
	assert(node);

	mxml_node_t* mesh_node = mxmlFindElement(node, node, "mesh", NULL, NULL, MXML_DESCEND);
	if(!mesh_node) {
		errorit("No mesh node in geometry!");
		return 1;
	}
	return RCBC_MiniXML_ProcessGeometries_Mesh(thing, mesh_node);
}

int RCBC_MiniXML_ProcessGeometries(RCBCThing *thing, mxml_node_t *node) {
	const char* id;

	debugit(DEBUG_LOW, "RCBC_MiniXML_ProcessGeometries");

	assert(thing);
	assert(node);

	debugit(DEBUG_LOW, "Freeing empty...");
	if(thing->geometries) {
		//RCBC_MeshFree(&(thing->geometries)); /* TODO: Probably not really needed but meh... */
	}

	for(node = node->child; node != NULL; node = node->next) {
		if(node->type == MXML_ELEMENT) {
			DumpNodeInfo(node);

			if(strcasecmp(node->value.element.name, "geometry") == 0) {
				return RCBC_MiniXML_ProcessGeometries_Geometry(thing, node);			
			}
		}
	}

	return 1;
}
