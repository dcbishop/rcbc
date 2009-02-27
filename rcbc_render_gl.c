#include <stdio.h>
#include "rcbc_render_gl.h"

int RCBC_GL_Init() {
	printf("Initilizing GL render...\n");
	return 0;
}

int RCBC_GL_Render(RCBCThing* thing) {
	printf("Rendering %p.\n", thing);
}
