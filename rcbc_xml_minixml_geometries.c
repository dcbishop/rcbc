#include <assert.h>

#include "rcbc_xml_minixml_geometries.h"
#include "rcbc_xml_minixml.h"
#include "console.h"

/* Reads a float array, such as verticies, normals, text cords, etc... */
FloatArray* RCBC_MiniXML_ProcessGeometries_Mesh_FloatArray(ModelTempory *tempory, Mesh *mesh, mxml_node_t *xnode) {
	DEBUG_M("Entering function...");

	const char* id = mxmlElementGetAttr(xnode, "id");
	const char* count_s = mxmlElementGetAttr(xnode, "count");
	int count = atoi(count_s);
	
	FloatArray* newarray = NEW(FloatArray, count);
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

	ListAdd(mesh->arrays, newarray);
	
	DEBUG_M("exiting function");
	return newarray;
}

int RCBC_MiniXML_ProcessGeometries_Mesh_Source(ModelTempory *tempory, Mesh *mesh, mxml_node_t *xnode) {
	DEBUG_M("Entering function...");
	const char* id = mxmlElementGetAttr(xnode, "id");
	
	for(xnode = xnode->child; xnode; xnode = xnode->next) {
		if(xnode->type == MXML_ELEMENT) {
			DumpNodeInfo(xnode);

			if(strcasecmp(xnode->value.element.name, "float_array") == 0) {
				FloatArray* array =  RCBC_MiniXML_ProcessGeometries_Mesh_FloatArray(tempory, mesh, xnode);
				Hookup* arrayhookup = NEW(Hookup, (char*)id, (void*)array);
				ListAdd(tempory->sources, arrayhookup);
			}
		}
	}
	#warning TODO: Remove the line below this line.
	DEBUG_M("Failed to find float array..."); 
	return 1;
}

int RCBC_MiniXML_ProcessGeometries_Mesh_Verticies(ModelTempory *tempory, Mesh *mesh, mxml_node_t *xnode) {
	DEBUG_M("Entering function...");
	
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

				DEHASH(source);

				Hookup* idhookup = NEW(Hookup, (char*)id, NULL);
				Hookup* sourcehookup = NEW(Hookup, (char*)source, (void*)&idhookup->ptr);
				ListAdd(tempory->sources, idhookup);
				ListAdd(tempory->sinks, sourcehookup);				
			}
		}
	}

	return 0;
}

UnsortedPolygons* RCBC_MiniXML_ProcessGeometries_Mesh_Polygons(ModelTempory *tempory, Mesh *mesh, mxml_node_t *xnode) {
	DEBUG_M("Entering function...");
	
	assert(tempory);
	assert(mesh);
	assert(xnode);

	mxml_node_t *node;	
}

UnsortedTriangles* RCBC_MiniXML_ProcessGeometries_Mesh_Triangles(ModelTempory *tempory, Mesh *mesh, mxml_node_t *xnode) {
	DEBUG_M("Entering function...");

	assert(tempory);
	assert(mesh);
	assert(xnode);

	mxml_node_t *node;
	const char* count_s = mxmlElementGetAttr(xnode, "count");
	const char* material = mxmlElementGetAttr(xnode, "material");

	int count = atoi(count_s);
	int inputs = 0;

	UnsortedTriangles* triangles = NEW(UnsortedTriangles, count);
	
	for(node = xnode->child; node != NULL; node = node->next) {
		DumpNodeInfo(node);
		if(node->type == MXML_ELEMENT) {
			if(strcasecmp(node->value.element.name, "input") == 0) {
				
				const char *semantic = mxmlElementGetAttr(node, "semantic");
				const char *source = mxmlElementGetAttr(node, "source");
				const char *offset_s = mxmlElementGetAttr(node, "offset");
				int offset = atoi(offset_s);

				DEHASH(source);

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

				Hookup* hookup = NEW(Hookup, (char*)source, ptr);
				ListAdd(tempory->sinks, hookup);

			} else if(strcasecmp(node->value.element.name, "p") == 0) {
				triangles->inputs = inputs;
				UnsortedTrianglesAllocateIndices(triangles);
				int value = -1;
				int i = 0;
				char* pch = strtok(node->child->value.opaque, " ");

				while(pch && i < count * inputs * 3) {
					sscanf(pch, "%d", &value);
					triangles->indices[i] = value;
					pch = strtok(NULL, " ");
					i++;
				}
			}
		}
	}

	triangles->inputs = inputs;
	triangles->ptr = (void**)&mesh->triangles;
	ListAdd(tempory->unsorted, triangles);

	if(material != NULL && material[0] != '\0') {
		Hookup* material_hookup = NEW(Hookup, (char*)material, (void*)&triangles->image);
		ListAdd(tempory->sinks, material_hookup);
	}

	return triangles;
}

int RCBC_MiniXML_ProcessGeometries_Mesh_Children(ModelTempory *tempory, Mesh *mesh, mxml_node_t *xnode) {
	DEBUG_M("Entering function...");
	if(!mesh) {
		ERROR("XML Passed NULL RCBC mesh... %s", SYMBOL_WARNING);
		return 1;
	} 
	if(!xnode) {
		ERROR("XML Passed NULL XML node... %s", SYMBOL_WARNING);
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
		#warning TODO: Convert polygons to triangle strips
		return RCBC_MiniXML_ProcessGeometries_Mesh_Polygons(tempory, mesh, xnode);
		ERROR("Model contains polygon data, convert to triangles.");
		return 1;
	} 
}

Mesh* RCBC_MiniXML_ProcessGeometries_Mesh(ModelTempory *tempory, mxml_node_t *xnode) {
	DEBUG_M("Entering function...");

	Mesh* last;
	assert(tempory);
	assert(xnode);

	Mesh* mesh = NEW(Mesh);
	ListAdd(tempory->model->geometries, mesh);

	mxml_node_t *child;
	const char *id = mxmlElementGetAttr(xnode, "id");

	for(child = xnode->child; child != NULL; child = child->next) {
		if(child->type == MXML_ELEMENT) {
			RCBC_MiniXML_ProcessGeometries_Mesh_Children(tempory, mesh, child);
		}
	}

	return mesh;
}


/**
 * Grabs the mesh from the geometry (COLLADA specs say there can be only one  
 * although a few nonmesh temporys are supported, they are usless to us and ignored)
 */ 
Mesh* RCBC_MiniXML_ProcessGeometries_Geometry(ModelTempory *tempory, mxml_node_t *node) {
	DEBUG(DEBUG_MEDIUM, "%sRCBC_MiniXML_ProcessGeometries_Geometry", COLOUR_LIGHT_BLUE);
	assert(tempory);
	assert(node);

	const char *id = mxmlElementGetAttr(node, "id");

	mxml_node_t* mesh_node = mxmlFindElement(node, node, "mesh", NULL, NULL, MXML_DESCEND);
	if(!mesh_node) {
		ERROR("No mesh node in geometry!");
		return NULL;
	}
	Mesh* mesh = RCBC_MiniXML_ProcessGeometries_Mesh(tempory, mesh_node);
	Hookup* hookup = NEW(Hookup, (char*)id, (void*)mesh);
	ListAdd(tempory->sources, hookup);
	return mesh;
}

int RCBC_MiniXML_ProcessGeometries(ModelTempory *tempory, mxml_node_t *node) {
	const char* id;

	DEBUG(DEBUG_MEDIUM, "RCBC_MiniXML_ProcessGeometries");

	assert(tempory);
	assert(node);

	#warning TODO: Free the mesh if it already exists for safety
	if(tempory->model->geometries) {
		//DEBUG(DEBUG_HIGH, "Freeing empty...");
		//RCBC_MeshFree(&(tempory->geometries));
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
