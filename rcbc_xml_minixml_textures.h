#ifndef _RCBC_MINIXML_TEXTURES_DEF
#define _RCBC_MINIXML_TEXTURES_DEF

#include <mxml.h>
#include "rcbc_data.h"

void RCBC_MiniXML_ProcessTextureImages(RCBC_Tempory *tempory, mxml_node_t *node);
void RCBC_MiniXML_ProcessTextureEffects(RCBC_Tempory *tempory, mxml_node_t *node);
void RCBC_MiniXML_ProcessTextureMaterial(RCBC_Tempory *tempory, mxml_node_t *node);
void RCBC_MiniXML_ProcessTextureEffects_Profile(RCBC_Tempory *tempory, mxml_node_t *node);

#endif
