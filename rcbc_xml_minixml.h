#ifndef _RCBC_MINIXML_DEF
#define _RCBC_MINIXML_DEF

#include <mxml.h>

#include "rcbc.h"

#define DEHASH(x) if(x[0] == '#'); x++

int RCBC_MiniXML_Init();
void DumpNodeInfo(mxml_node_t *node);
int RCBC_MiniXML_Load(Model* model, List* images, char* filename);

#endif
