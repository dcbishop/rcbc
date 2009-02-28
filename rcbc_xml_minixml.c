#include <stdio.h>

#include <mxml.h>

#include "rcbc_xml_minixml.h"
#include "console.h"

int RCBC_MiniXML_Init() {
	logit("Initilizing MiniXML...");
	return 0;
}

RCBCThing* RCBC_MiniXML_Load(char* filename) {
	logit("[MINIXML]: Opening '%s'...", filename);
	FILE *fp;
	mxml_node_t *tree;
	fp = fopen(filename, "r");
	if(!fp) {
		errorit("[MINIXML]: Error opening %s... %s", filename, SYMBOL_FATAL);
		return NULL;
	}
	logit("[MINIXML]: Parsing '%s'...", filename);
	tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
	fclose(fp);
	
	if(!tree) {
		errorit("[MINIXML]: Error parsing %s... %s", filename, SYMBOL_FATAL);
		return NULL;
	}
	logit("[MINIXML]: Successfuly loaded... %s", SYMBOL_SMILEY);

	RCBCThing* thing = malloc(sizeof(RCBCThing));

	mxmlDelete(tree);
	return thing;
}
