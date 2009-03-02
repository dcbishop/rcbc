#include <stdlib.h>

#include "rcbc.h"
#include "rcbc_defaults.h"
#include "rcbc_data.h"
#include "console.h"

#ifdef RCBC_RENDER_OPENGL
#include "rcbc_render_gl.h"
#define RCBC_RENDER_INIT RCBC_GL_Init
#define RCBC_RENDER_RENDER RCBC_GL_Draw
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

#ifdef RCBC_XML_MINIXML
#include "rcbc_xml_minixml.h"
#define RCBC_XML_INIT RCBC_MiniXML_Init
#define RCBC_XML_LOAD RCBC_MiniXML_Load
#endif

static RCBCPlugins rcbc_plugins;
static int rcbc_initilized = 0;

int RCBC_Init() {
	logit("RCBC initilizing...");
	rcbc_plugins.render_draw = (void*)RCBC_RENDER_RENDER;
	rcbc_plugins.xml_load = (void*)RCBC_XML_LOAD;

	logit("Initilizing render...");
	if(RCBC_RENDER_INIT()) {
		errorit("Failed to initilize XML parser... %s", SYMBOL_FATAL);
		return 1;
	}

	logit("Initilizing XML parser...");
	if(RCBC_XML_INIT()) {
		errorit("Failed to initilize XML parser... %s", SYMBOL_FATAL);
		return 1;
	}

	rcbc_initilized = 1;
	logit("RCBC initilzed successfuly %s", SYMBOL_SMILEY);
	return 0;
}

RCBCThing* RCBC_LoadFile(const char* filename) {
	logit("RCBC loading '%s'...", filename);

	if(!rcbc_initilized) {
		errorit("Attempted to use uninitilized RCBC... %s", SYMBOL_FATAL);
		return NULL;
	}

	RCBCThing* thing = RCBC_ThingGenerate();

	rcbc_plugins.xml_load(thing, filename);

	return thing;
}

int RCBC_Render(const RCBCThing* thing) {
	if(!rcbc_initilized) {
		errorit("Attempted to use uninitilized RCBC... %s", SYMBOL_FATAL);
		return 1;
	}

	rcbc_plugins.render_draw(thing);	
}
