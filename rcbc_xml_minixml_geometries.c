#include <assert.h>

#include "rcbc_xml_minixml_geometries.h"
#include "rcbc_data.h"
#include "console.h"

/* Reads a float array, such as verticies, normals, text cords, etc... */
RCBC_FloatArray* RCBC_MiniXML_ProcessGeometries_Mesh_FloatArray(RCBCThing *thing, RCBCMesh *mesh, mxml_node_t *xnode) {
	const char* id = mxmlElementGetAttr(xnode, "id");
	const char* count_s = mxmlElementGetAttr(xnode, "count");
	int count = atoi(count_s);
	
	RCBC_FloatArray* newarray = RCBC_FloatArrayGenerate(count);
	if(!newarray) {
		return NULL;
	}

	float f = 0.0f;
	int i = 0;
	char* pch = strtok(xnode->child->value.opaque, " ");
	while(pch && i < count) {
		sscanf(pch, "%f", &f);
		newarray->values[i] = f;
		pch = strtok(NULL, " ");
		i++;
	}

	//RCBC_Hookup* hookup; 
	//hookup = RCBC_HookupGenerate((char*)id, (void*)newarray);

	LLAdd(&mesh->arrays, newarray);
	//LLAdd(&thing->sources, hookup);

	return newarray;
}

int RCBC_MiniXML_ProcessGeometries_Mesh_Source(RCBCThing *thing, RCBCMesh *mesh, mxml_node_t *xnode) {
	debugit(DEBUG_LOW, "%sRCBC_MiniXML_ProcessGeometries_Mesh_Source", COLOUR_LIGHT_BLUE);
	const char* id = mxmlElementGetAttr(xnode, "id");
	debugit(DEBUG_LOW, "\t\tID: %s", id);
	for(xnode = xnode->child; xnode; xnode = xnode->next) {
		if(xnode->type == MXML_ELEMENT) {
			DumpNodeInfo(xnode);

			if(strcasecmp(xnode->value.element.name, "float_array") == 0) {
				RCBC_FloatArray* array =  RCBC_MiniXML_ProcessGeometries_Mesh_FloatArray(thing, mesh, xnode);
				RCBC_Hookup* arrayhookup = RCBC_HookupGenerate((char*)id, (void*)array);
				LLAdd(&thing->sources, arrayhookup);
			}
		}
	}
	errorit("Failed to find float array...");
	return 1;
}

int RCBC_MiniXML_ProcessGeometries_Mesh_Verticies(RCBCThing *thing, RCBCMesh *mesh, mxml_node_t *xnode) {
	debugit(DEBUG_LOW, "%sRCBC_MiniXML_ProcessGeometries_Mesh_Polygons: Verticies", COLOUR_LIGHT_BLUE);

	assert(mesh); 
	assert(xnode);

	mxml_node_t *node;

	const char *source;
	const char *semantic;
	const char *id = mxmlElementGetAttr(xnode, "id");

	for(node = xnode->child; node != NULL; node = node->next) {
		DumpNodeInfo(node);
		if(node->type == MXML_ELEMENT) {
			if(strcasecmp(node->value.element.name, "input") == 0) {
				semantic = mxmlElementGetAttr(node, "semantic");
				source = mxmlElementGetAttr(node, "source");

				if(source[0] == '#') {
					source = source+1;
				}

				RCBC_Hookup* idhookup = RCBC_HookupGenerate((char*)id, NULL);
				RCBC_Hookup* sourcehookup = RCBC_HookupGenerate((char*)source, (void*)&idhookup->ptr);
				LLAdd(&thing->sources, idhookup);
				LLAdd(&thing->sinks, sourcehookup);				
			}
		}
	}
	debugit(DEBUG_LOW, "%sEND", COLOUR_LIGHT_BLUE);
	return 0;
}

RCBC_Triangles* RCBC_MiniXML_ProcessGeometries_Mesh_Triangles(RCBCThing *thing, RCBCMesh *mesh, mxml_node_t *xnode) {
	debugit(DEBUG_LOW, "%sRCBC_MiniXML_ProcessGeometries_Mesh_Polygons", COLOUR_LIGHT_BLUE);

	assert(mesh);
	assert(xnode);

	mxml_node_t *node;
	const char* count_s = mxmlElementGetAttr(xnode, "count");
	int count = atoi(count_s);

	RCBC_Triangles* triangles = RCBC_TrianglesGenerate(count);
	
	for(node = xnode->child; node != NULL; node = node->next) {
		DumpNodeInfo(node);
		if(node->type == MXML_ELEMENT) {
			if(strcasecmp(node->value.element.name, "input") == 0) {
				
				
				const char *semantic = mxmlElementGetAttr(node, "semantic");
				const char *source = mxmlElementGetAttr(node, "source");
				const char *offset_s = mxmlElementGetAttr(node, "offset");
				int offset = atoi(offset_s);

				if(source[0] == '#') {
					source = source+1;
				}

				void* ptr = NULL;

				// TODO!	
				if(strcasecmp(semantic, "VERTEX") == 0) {
					ptr = &triangles->vertices;
				} else if(strcasecmp(semantic, "NORMALS") == 0) {
					ptr = &triangles->normals;
				}

				RCBC_Hookup* hookup = RCBC_HookupGenerate((char*)source, ptr);
				LLAdd(&mesh->sinks, thing);

			} else if(strcasecmp(node->value.element.name, "p") == 0) {
				RCBC_TrianglesAllocateIndex(triangles);
				int value = -1;
				int i = 0;
				char* pch = strtok(xnode->child->value.opaque, " ");
				while(pch && i < count) {
					sscanf(pch, "%d", &value);
					triangles->index[i] = value;
					pch = strtok(NULL, " ");
					i++;
				}
			}
		}
	}
	debugit(DEBUG_LOW, "function end...");
	return triangles;
}

int RCBC_MiniXML_ProcessGeometries_Mesh_Children(RCBCThing *thing, RCBCMesh *mesh, mxml_node_t *xnode) {
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
		return RCBC_MiniXML_ProcessGeometries_Mesh_Source(thing, mesh, xnode);
	} else if(strcasecmp(xnode->value.element.name, "vertices") == 0) {
		return RCBC_MiniXML_ProcessGeometries_Mesh_Verticies(thing, mesh, xnode);
	} else if(strcasecmp(xnode->value.element.name, "triangles") == 0) {
		if(RCBC_MiniXML_ProcessGeometries_Mesh_Triangles(thing, mesh, xnode)) {
			return 0;
		} else {
			return 1;
		}
	} else if(strcasecmp(xnode->value.element.name, "polygons") == 0) {
		//return RCBC_MiniXML_ProcessGeometries_Mesh_Polygons(mesh, xnode);
		errorit("Model contains polygon data, convert to triangles.");
		return 1;
	} 
}

RCBCMesh* RCBC_MiniXML_ProcessGeometries_Mesh(RCBCThing *thing, mxml_node_t *xnode) {
	RCBCMesh* last;

	assert(thing);
	assert(xnode);

	debugit(DEBUG_LOW, "%sRCBC_MiniXML_ProcessGeometries_Mesh", COLOUR_LIGHT_BLUE);

	RCBCMesh* mesh = RCBC_MeshGenerate();
	LLAdd(&thing->geometries, mesh);

	mxml_node_t *child;
	const char *id = mxmlElementGetAttr(xnode, "id");
	debugit(DEBUG_LOW, "MESH ID: '%s'", id);
	for(child = xnode->child; child != NULL; child = child->next) {
		if(child->type == MXML_ELEMENT) {
			RCBC_MiniXML_ProcessGeometries_Mesh_Children(thing, mesh, child);
		}
	}


	/*debugit(DEBUG_LOW, "Prejoin...");
	LLJoin(&thing->sources, mesh->sources);
	LLJoin(&thing->sinks, mesh->sinks);
	debugit(DEBUG_LOW, "Postjoin...");*/

	return mesh;
}


/* Grabs the mesh from the geometry (COLLADA specs say there can be only one  
 * although a few nonmesh things are supported, they are usless to us and ignored) */ 
RCBCMesh* RCBC_MiniXML_ProcessGeometries_Geometry(RCBCThing *thing, mxml_node_t *node) {
	assert(thing);
	assert(node);

	const char *id = mxmlElementGetAttr(node, "id");

	mxml_node_t* mesh_node = mxmlFindElement(node, node, "mesh", NULL, NULL, MXML_DESCEND);
	if(!mesh_node) {
		errorit("No mesh node in geometry!");
		return NULL;
	}
	RCBCMesh* mesh = RCBC_MiniXML_ProcessGeometries_Mesh(thing, mesh_node);
	RCBC_Hookup* hookup = RCBC_HookupGenerate((char*)id, (void*)mesh);
	LLAdd(&thing->sources, mesh);
	return mesh;
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
				if(!RCBC_MiniXML_ProcessGeometries_Geometry(thing, node)) {
					return 1;
				}
			}
		}
	}

	return 1;
}
