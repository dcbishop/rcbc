#ifndef _RCBC_MINIXML_DEF
#define _RCBC_MINIXML_DEF

#include <mxml.h>

#include "rcbc.h"

int RCBC_MiniXML_Init();
void DumpNodeInfo(mxml_node_t *node);
int RCBC_MiniXML_Load(RCBCThing* thing, char* filename);

#endif
