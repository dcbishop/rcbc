#ifndef _RCBC_MINIXML_VISUALSCENE_DEF
#define _RCBC_MINIXML_VISUALSCENE_DEF

#include <mxml.h>

#include "rcbc.h"
#include "rcbc_data.h"

void RCBC_MiniXML_ProcessVisualScene_Node_Scale(SceneNode *rnode, mxml_node_t *xnode);
void RCBC_MiniXML_ProcessVisualScene_Node_Translate(SceneNode *rnode, mxml_node_t *xnode);
Rotate* RCBC_MiniXML_ProcessVisualScene_Node_Rotate(SceneNode *rnode, mxml_node_t *xnode);
void RCBC_MiniXML_ProcessVisualScene_Node_Children(ModelTempory *tempory, SceneNode *rnode, mxml_node_t *xnode);
void RCBC_MiniXML_ProcessVisualScene_Node(ModelTempory *tempory, SceneNode **rnode, mxml_node_t *xnode);
void RCBC_MiniXML_ProcessVisualScene(ModelTempory *tempory, mxml_node_t *node);

#endif
