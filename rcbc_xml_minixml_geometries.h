/* Loads a COLLADA geometry in from the XML (this is a polygon mesh) */
#ifndef _RCBC_MINIXML_GEOMETRIES_DEF
#define _RCBC_MINIXML_GEOMETRIES_DEF

#include <mxml.h>

#include "rcbc.h"
#include "rcbc_data.h"

RCBC_FloatArray* RCBC_MiniXML_ProcessGeometries_Mesh_FloatArray(RCBCTempory *tempory, RCBCMesh *mesh, mxml_node_t *xnode);
int RCBC_MiniXML_ProcessGeometries_Mesh_Source(RCBCTempory *tempory, RCBCMesh *mesh, mxml_node_t *xnode);
int RCBC_MiniXML_ProcessGeometries_Mesh_Verticies(RCBCTempory *tempory, RCBCMesh *mesh, mxml_node_t *xnode);
RCBC_TrianglesUnsorted* RCBC_MiniXML_ProcessGeometries_Mesh_Triangles(RCBCTempory *tempory, RCBCMesh *mesh, mxml_node_t *xnode);
int RCBC_MiniXML_ProcessGeometries_Mesh_Children(RCBCTempory *tempory, RCBCMesh *mesh, mxml_node_t *xnode);
RCBCMesh* RCBC_MiniXML_ProcessGeometries_Mesh(RCBCTempory *tempory, mxml_node_t *xnode);
RCBCMesh* RCBC_MiniXML_ProcessGeometries_Geometry(RCBCTempory *tempory, mxml_node_t *node);
int RCBC_MiniXML_ProcessGeometries(RCBCTempory *tempory, mxml_node_t *node);

#endif
