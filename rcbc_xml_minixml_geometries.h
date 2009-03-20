/* Loads a COLLADA geometry in from the XML (this is a polygon mesh) */
#ifndef _RCBC_MINIXML_GEOMETRIES_DEF
#define _RCBC_MINIXML_GEOMETRIES_DEF

#include <mxml.h>

#include "rcbc.h"
#include "rcbc_data.h"

FloatArray* RCBC_MiniXML_ProcessGeometries_Mesh_FloatArray(ModelTempory *tempory, Mesh *mesh, mxml_node_t *xnode);
int RCBC_MiniXML_ProcessGeometries_Mesh_Source(ModelTempory *tempory, Mesh *mesh, mxml_node_t *xnode);
int RCBC_MiniXML_ProcessGeometries_Mesh_Verticies(ModelTempory *tempory, Mesh *mesh, mxml_node_t *xnode);
UnsortedTriangleData* RCBC_MiniXML_ProcessGeometries_Mesh_Triangles(ModelTempory *tempory, Mesh *mesh, mxml_node_t *xnode);
int RCBC_MiniXML_ProcessGeometries_Mesh_Children(ModelTempory *tempory, Mesh *mesh, mxml_node_t *xnode);
Mesh* RCBC_MiniXML_ProcessGeometries_Mesh(ModelTempory *tempory, mxml_node_t *xnode);
Mesh* RCBC_MiniXML_ProcessGeometries_Geometry(ModelTempory *tempory, mxml_node_t *node);
int RCBC_MiniXML_ProcessGeometries(ModelTempory *tempory, mxml_node_t *node);

#endif
