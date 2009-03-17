#include <stdio.h>
#include "rcbc_render_textinfo.h"
#include "console.h"

int RCBC_TextInfo_Init() {
	LOG("Initilized textinfo renderer...");
	return 0;
}

int RCBC_TextInfo_Render(RCBC_Model* thing) {
	LOG("Rendering %p.", thing);
}
