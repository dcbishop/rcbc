#ifndef _RCBC_DEF
#define _RCBC_DEF

/* Default paths */
#define DIRECTORY_TEXTURES "./data/textures/"
#define DIRECTORY_MODELS "./data/models/"

#include "rcbc_data.h"

/* This contains the swapable components, incase you want to replace the XML
 * library or whatever, you could also do models like replace the render with 
 * somemodel that gives a text dump of file information or a wrapper function
 * that draws a cube around the model or whatever. In theory anyway, for now
 * not all of this might actually be replacable. */
typedef struct RCBCPlugins {
	int (*render_draw)(const Model* model);
	Model* (*xml_load)(Model* model, List* images, const char* filename);
} RCBCPlugins;


int RCBC_Init();
Model* RCBC_LoadFile(const char* filename, List* images);
int RCBC_Render(const Model* model);

#endif
