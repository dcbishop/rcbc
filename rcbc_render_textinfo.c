#include <stdio.h>
#include "rcbc_render_textinfo.h"

int RCBC_TextInfo_Init() {
	printf("Initilized text renderer...\n");
	return 0;
}

int RCBC_TextInfo_Render(RCBCThing* thing) {
	printf("[TEXTINFO]: Rendering %p.\n", thing);
}
