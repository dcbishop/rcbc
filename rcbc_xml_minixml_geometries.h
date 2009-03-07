/* Loads a COLLADA geometry in from the XML (this is a polygon mesh) */
#ifndef _RCBC_MINIXML_GEOMETRIES_DEF
#define _RCBC_MINIXML_GEOMETRIES_DEF

#include <mxml.h>

#include "rcbc.h"
#include "rcbc_data.h"

RCBC_FloatArray* RCBC_MiniXML_ProcessGeometries_Mesh_FloatArray(RCBCThing *thing, RCBCMesh *mesh, mxml_node_t *xnode);
int RCBC_MiniXML_ProcessGeometries_Mesh_Source(RCBCThing *thing, RCBCMesh *mesh, mxml_node_t *xnode);
int RCBC_MiniXML_ProcessGeometries_Mesh_Children(RCBCThing *thing, RCBCMesh *mesh, mxml_node_t *xnode);
RCBCMesh* RCBC_MiniXML_ProcessGeometries_Mesh(RCBCThing *thing, mxml_node_t *xnode);
RCBCMesh* RCBC_MiniXML_ProcessGeometries_Geometry(RCBCThing *thing, mxml_node_t *node);
int RCBC_MiniXML_ProcessGeometries(RCBCThing *thing, mxml_node_t *node);

#endif
