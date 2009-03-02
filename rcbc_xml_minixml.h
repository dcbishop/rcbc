#ifndef _RCBC_MINIXML_DEF
#define _RCBC_MINIXML_DEF

#include <mxml.h>

#include "rcbc.h"

int RCBC_MiniXML_Init();
int RCBC_MiniXML_Load(RCBCThing* thing, char* filename);
void RCRB_MiniXML_ProcessVisualScene_Node(RCBCNode **rnode, mxml_node_t *xnode);
#endif
