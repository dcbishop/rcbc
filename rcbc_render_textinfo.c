#include <stdio.h>
#include "rcbc_render_textinfo.h"
#include "console.h"

/**
 * @return 0
 */
int RCBC_TextInfo_Init() {
	LOG("Initilized textinfo renderer...");
	return 0;
}

/**
 * Prints to model's pointer to the console.
 * @return 0
 */
int RCBC_TextInfo_Render(Model* model) {
	LOG("Rendering %p.", model);
	return 0;
}
