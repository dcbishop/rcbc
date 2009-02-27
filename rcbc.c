#include <stdlib.h>

#include "rcbc.h"
#include "rcbc_defaults.h"
#include "console.h"

#ifdef RCBC_RENDER_OPENGL
#include "rcbc_render_gl.h"
#define RCBC_RENDER_INIT RCBC_GL_Init
#define RCBC_RENDER_RENDER RCBC_GL_Render
#endif

#ifdef RCBC_RENDER_TEXTINFO
#include "rcbc_render_textinfo.h"
#define RCBC_RENDER_INIT RCBC_TextInfo_Init
#define RCBC_RENDER_RENDER RCBC_TextInfo_Render
#endif

#ifdef RCBC_XML_TINYXML
#include "rcbc_xml_tinyxml.h"
#define RCBC_XML_INIT RCBC_TinyXML_Init
#define RCBC_XML_LOAD RCBC_TinyXML_Load
#endif

static RCBCPlugins rcbc_plugins;
static int rcbc_initilized = 0;

int RCBC_Init() {
	rcbc_plugins.render = (void*)RCBC_RENDER_RENDER;

	logit("Initilizing XML parser...");
	if(RCBC_XML_INIT()) {
		errorit("Failed to initilize XML parser... %s", SYMBOL_FATAL);
		return 1;
	}

	logit("Initilizing render...");
	if(RCBC_RENDER_INIT()) {
		errorit("Failed to initilize XML parser... %s", SYMBOL_FATAL);
		return 1;
	}

	rcbc_initilized = 1;
	return 0;
}

RCBCThing* RCBC_LoadFile(const char* filename) {
	if(!rcbc_initilized && !RCBC_Init()) {
		return NULL;
	}

	RCBCThing* thing = malloc(sizeof(RCBCThing));
	if(!thing) {
		return NULL;
	}

	return thing;
}

int RCBC_Render(const RCBCThing* thing) {
	rcbc_plugins.render(thing);	
}
