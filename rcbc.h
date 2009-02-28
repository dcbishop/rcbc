#ifndef _RCBC_DEF
#define _RCBC_DEF

/* A container for a Collada file */
typedef struct RCBCThing {
	int yay;
} RCBCThing;

/* This contains the swapable components, incase you want to replace the XML
 * library or whatever, you could also do things like replace the render with 
 * something that gives a text dump of file information or a wrapper function
 * that draws a cube around the model or whatever. In theory anyway, for now
 * not all of this might actually be replacable. */
typedef struct RCBCPlugins {
	int (*render_draw)(const RCBCThing* thing);
	RCBCThing* (*xml_load)(const char* filename);
} RCBCPlugins;

int RCBC_Init();
RCBCThing* RCBC_LoadFile(const char* filename);
int RCBC_Render(const RCBCThing* thing);

#endif
