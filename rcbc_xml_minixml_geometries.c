#include <assert.h>

#include "rcbc_xml_minixml_geometries.h"
#include "rcbc_data.h"
#include "console.h"

/* Reads a float array, such as verticies, normals, text cords, etc... */
RCBC_FloatArray* RCBC_MiniXML_ProcessGeometries_Mesh_FloatArray(RCBCTempory *tempory, RCBCMesh *mesh, mxml_node_t *xnode) {
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

//	RCBC_Hookup* hookup = RCBC_HookupGenerate((char*)id, (void*)newarray);

	LLAdd(&mesh->arrays, newarray);
//	LLAdd(&tempory->sources, hookup);

	return newarray;
}

int RCBC_MiniXML_ProcessGeometries_Mesh_Source(RCBCTempory *tempory, RCBCMesh *mesh, mxml_node_t *xnode) {
	debugit(DEBUG_MEDIUM, "%sRCBC_MiniXML_ProcessGeometries_Mesh_Source", COLOUR_LIGHT_BLUE);
	const char* id = mxmlElementGetAttr(xnode, "id");
	debugit(DEBUG_HIGH, "\t\tID: %s", id);
	for(xnode = xnode->child; xnode; xnode = xnode->next) {
		if(xnode->type == MXML_ELEMENT) {
			DumpNodeInfo(xnode);

			if(strcasecmp(xnode->value.element.name, "float_array") == 0) {
				RCBC_FloatArray* array =  RCBC_MiniXML_ProcessGeometries_Mesh_FloatArray(tempory, mesh, xnode);
				RCBC_Hookup* arrayhookup = RCBC_HookupGenerate((char*)id, (void*)array);
				LLAdd(&tempory->sources, arrayhookup);
			}
		}
	}
	debugit(DEBUG_MEDIUM, "Failed to find float array..."); /* TODO: Some of these are normal, should handle these cases */
	return 1;
}

int RCBC_MiniXML_ProcessGeometries_Mesh_Verticies(RCBCTempory *tempory, RCBCMesh *mesh, mxml_node_t *xnode) {
	debugit(DEBUG_MEDIUM, "%sRCBC_MiniXML_ProcessGeometries_Mesh_Verticies", COLOUR_LIGHT_BLUE);

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
				debugit(DEBUG_VERY_HIGH, "%sINFOMRATION: id:'%s' source:'%s'", COLOUR_LIGHT_RED, id, source);
				LLAdd(&tempory->sources, idhookup);
				LLAdd(&tempory->sinks, sourcehookup);				
			}
		}
	}

	return 0;
}

RCBC_TrianglesUnsorted* RCBC_MiniXML_ProcessGeometries_Mesh_Triangles(RCBCTempory *tempory, RCBCMesh *mesh, mxml_node_t *xnode) {
	debugit(DEBUG_MEDIUM, "%sRCBC_MiniXML_ProcessGeometries_Mesh_Polygons", COLOUR_LIGHT_BLUE);

	assert(mesh);
	assert(xnode);

	mxml_node_t *node;
	const char* count_s = mxmlElementGetAttr(xnode, "count");
	int count = atoi(count_s);
	int inputs = 0;

	RCBC_TrianglesUnsorted* triangles = RCBC_TrianglesUnsortedGenerate(count);
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
				inputs++;
				if(strcasecmp(semantic, "VERTEX") == 0) {
					ptr = &(triangles->vertices);
					triangles->vertices_offset = offset;
				} else if(strcasecmp(semantic, "NORMAL") == 0) {
					ptr = &(triangles->normals);
					triangles->normals_offset = offset;
				} else if(strcasecmp(semantic, "TEXCOORD") == 0) {
					ptr = &(triangles->texcoords);
					triangles->texcoords_offset = offset;
				}

				RCBC_Hookup* hookup = RCBC_HookupGenerate((char*)source, ptr);
				LLAdd(&tempory->sinks, hookup);

			} else if(strcasecmp(node->value.element.name, "p") == 0) {
				triangles->inputs = inputs;
				RCBC_TrianglesUnsortedAllocateIndices(triangles);
				int value = -1;
				int i = 0;
				char* pch = strtok(node->child->value.opaque, " ");

				while(pch && i < count * inputs * 3) {
					sscanf(pch, "%d", &value);
					triangles->indices[i] = value; /* SEGFAULT */
					pch = strtok(NULL, " ");
					i++;
				}
			}
		}
	}

	triangles->inputs = inputs;
	triangles->ptr = (void**)&mesh->triangles;
	LLAdd(&tempory->unsorted, triangles);
	return triangles;
}

int RCBC_MiniXML_ProcessGeometries_Mesh_Children(RCBCTempory *tempory, RCBCMesh *mesh, mxml_node_t *xnode) {
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
		return RCBC_MiniXML_ProcessGeometries_Mesh_Source(tempory, mesh, xnode);
	} else if(strcasecmp(xnode->value.element.name, "vertices") == 0) {
		return RCBC_MiniXML_ProcessGeometries_Mesh_Verticies(tempory, mesh, xnode);
	} else if(strcasecmp(xnode->value.element.name, "triangles") == 0) {
		if(!RCBC_MiniXML_ProcessGeometries_Mesh_Triangles(tempory, mesh, xnode)) {
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

RCBCMesh* RCBC_MiniXML_ProcessGeometries_Mesh(RCBCTempory *tempory, mxml_node_t *xnode) {
	debugit(DEBUG_MEDIUM, "%sRCBC_MiniXML_ProcessGeometries_Mesh", COLOUR_LIGHT_BLUE);

	RCBCMesh* last;
	assert(tempory);
	assert(xnode);

	RCBCMesh* mesh = RCBC_MeshGenerate();
	LLAdd(&tempory->thing->geometries, mesh);

	mxml_node_t *child;
	const char *id = mxmlElementGetAttr(xnode, "id");
	debugit(DEBUG_HIGH, "MESH ID: '%s'", id);

	for(child = xnode->child; child != NULL; child = child->next) {
		if(child->type == MXML_ELEMENT) {
			RCBC_MiniXML_ProcessGeometries_Mesh_Children(tempory, mesh, child);
		}
	}

	return mesh;
}


/* Grabs the mesh from the geometry (COLLADA specs say there can be only one  
 * although a few nonmesh temporys are supported, they are usless to us and ignored) */ 
RCBCMesh* RCBC_MiniXML_ProcessGeometries_Geometry(RCBCTempory *tempory, mxml_node_t *node) {
	debugit(DEBUG_MEDIUM, "%sRCBC_MiniXML_ProcessGeometries_Geometry", COLOUR_LIGHT_BLUE);
	assert(tempory);
	assert(node);

	const char *id = mxmlElementGetAttr(node, "id");

	mxml_node_t* mesh_node = mxmlFindElement(node, node, "mesh", NULL, NULL, MXML_DESCEND);
	if(!mesh_node) {
		errorit("No mesh node in geometry!");
		return NULL;
	}
	RCBCMesh* mesh = RCBC_MiniXML_ProcessGeometries_Mesh(tempory, mesh_node);
	RCBC_Hookup* hookup = RCBC_HookupGenerate((char*)id, (void*)mesh);
	LLAdd(&tempory->sources, hookup);
	return mesh;
}

int RCBC_MiniXML_ProcessGeometries(RCBCTempory *tempory, mxml_node_t *node) {
	const char* id;

	debugit(DEBUG_MEDIUM, "RCBC_MiniXML_ProcessGeometries");

	assert(tempory);
	assert(node);


	if(tempory->thing->geometries) {
		//debugit(DEBUG_HIGH, "Freeing empty...");
		//RCBC_MeshFree(&(tempory->geometries)); /* TODO: Probably not really needed but meh... */
	}

	for(node = node->child; node != NULL; node = node->next) {
		if(node->type == MXML_ELEMENT) {
			DumpNodeInfo(node);

			if(strcasecmp(node->value.element.name, "geometry") == 0) {
				if(!RCBC_MiniXML_ProcessGeometries_Geometry(tempory, node)) {
					return 1;
				}
			}
		}
	}

	return 1;
}
