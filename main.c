#include <stdlib.h>
#include <stdio.h>

#include "rcbc.h"

extern struct RCBCPlugins rcbc_plugins;

int main(int argc, char** argv) {
	RCBC_Init();
	RCBCThing* thing = RCBC_LoadFile("samples/blender-default-cube.dae");
	RCBC_Render(NULL);

//	rcbc_plugins.render(thing);

	exit(EXIT_SUCCESS);
}
