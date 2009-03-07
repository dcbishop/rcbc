#ifndef _RCBC_MINIXML_VISUALSCENE_DEF
#define _RCBC_MINIXML_VISUALSCENE_DEF

#include <mxml.h>

#include "rcbc.h"
#include "rcbc_data.h"

void RCBC_MiniXML_ProcessVisualScene_Node_Scale(RCBCNode *rnode, mxml_node_t *xnode);
void RCBC_MiniXML_ProcessVisualScene_Node_Translate(RCBCNode *rnode, mxml_node_t *xnode);
RCBCNode_Rotate* RCBC_MiniXML_ProcessVisualScene_Node_Rotate(RCBCNode *rnode, mxml_node_t *xnode);
void RCBC_MiniXML_ProcessVisualScene_Node_Children(RCBCThing *thing, RCBCNode *rnode, mxml_node_t *xnode);
void RCBC_MiniXML_ProcessVisualScene_Node(RCBCThing *thing, RCBCNode **rnode, mxml_node_t *xnode);
void RCBC_MiniXML_ProcessVisualScene(RCBCThing *thing, mxml_node_t *node);

#endif
