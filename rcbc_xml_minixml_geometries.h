/* Loads a COLLADA geometry in from the XML (this is a polygon mesh) */
#ifndef _RCBC_MINIXML_GEOMETRIES_DEF
#define _RCBC_MINIXML_GEOMETRIES_DEF

#include <mxml.h>

#include "rcbc.h"
#include "rcbc_data.h"

int RCRB_MiniXML_ProcessGeometries_Geometry(RCBCThing *thing, mxml_node_t *node);
int RCRB_MiniXML_ProcessGeometries(RCBCThing *thing, mxml_node_t *node);
int RCRB_MiniXML_ProcessGeometries_Mesh(RCBCMesh **mesh, mxml_node_t *xnode);

#endif
