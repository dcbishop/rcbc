#ifndef _RCBC_MINIXML_TEXTURES_DEF
#define _RCBC_MINIXML_TEXTURES_DEF

#include <mxml.h>
#include "rcbc_data.h"

void RCBC_MiniXML_ProcessTextureImages(ModelTempory *tempory, mxml_node_t *node);
void RCBC_MiniXML_ProcessTextureEffects(ModelTempory *tempory, mxml_node_t *node);
void RCBC_MiniXML_ProcessTextureMaterial(ModelTempory *tempory, mxml_node_t *node);
void RCBC_MiniXML_ProcessTextureEffects_Profile(ModelTempory *tempory, mxml_node_t *node, Hookup* fx_hookup);
void RCBC_MiniXML_ProcessTextureEffects_Newparam(ModelTempory *tempory, mxml_node_t *node, Hookup* fx_hookup);

#endif
