#include <stdio.h>
#include "rcbc_render_gl.h"
#include "console.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glut.h>

int RCBC_GL_Init() {
	logit("Initilizing GL render...");
	return 0;
}

void RCBC_GL_Draw_Node(RCBCNode* node) {
	glTranslatef(node->translate[0], node->translate[1], node->translate[2]);

	glPushMatrix();
		glScalef(node->scale[0], node->scale[1], node->scale[2]);
		glutWireCube(1.0f);
	glPopMatrix();
}

int RCBC_GL_Draw(RCBCThing* thing) {
	RCBCNode* head = thing->visual_scene;
	RCBCNode* current = head;

	while(current) {
		RCBC_GL_Draw_Node(current);
		while(current->child) {
			current = current->child;
		}

		if(current->next) {
			current = current->next;
		} else {
			current = current->parent;
		}
	}

	return 0;
}
