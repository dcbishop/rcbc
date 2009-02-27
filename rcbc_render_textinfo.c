#include <stdio.h>
#include "rcbc_render_textinfo.h"

int RCBC_TextInfo_Init() {
	logit("Initilized textinfo renderer...");
	return 0;
}

int RCBC_TextInfo_Render(RCBCThing* thing) {
	logit("[TEXTINFO]: Rendering %p.", thing);
}
