#ifndef _RCBC_MINIXML_TEXTURES_DEF
#define _RCBC_MINIXML_TEXTURES_DEF

#include <mxml.h>
#include "rcbc_data.h"

void RCBC_MiniXML_ProcessTextureImages(RCBCTempory *tempory, mxml_node_t *node);
void RCBC_MiniXML_ProcessTextureEffects(RCBCTempory *tempory, mxml_node_t *node);
void RCBC_MiniXML_ProcessTextureMaterial(RCBCTempory *tempory, mxml_node_t *node);
void RCBC_MiniXML_ProcessTextureEffects_Profile(RCBCTempory *tempory, mxml_node_t *node);

#endif
