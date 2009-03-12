#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include "rcbc_render_gl.h"
#include "console.h"

#define PI 3.14152f

int RCBC_GL_Init() {
	logit("Initilizing GL render...");
	return 0;
}

void RCBC_GL_Draw_Node(RCBCNode* node) {
	LLNode* itr;

	glTranslatef(node->translate[0], node->translate[1], node->translate[2]);
	glPushMatrix();

	for(itr = node->rotations; itr; itr=itr->next) {
		RCBCNode_Rotate* rotation = itr->data;
		glRotatef(rotation->angle, rotation->x, rotation->y, rotation->z);
	}

	glScalef(node->scale[0], node->scale[1], node->scale[2]);
	
	RCBCMesh* mesh = node->mesh;
	RCBC_Triangles* triangles;
	if(!mesh || !(triangles = mesh->triangles)) {
		glColor3f(1.0f, 0.0f, 0.0f);
		glutWireSphere(1.0f, 5, 5);
	} else {
		glColor3f(1.0f, 1.0f, 1.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, triangles->vertices->values);
		if(triangles->normals) {
			glNormalPointer(GL_FLOAT, 0, triangles->normals->values);
		} else {

		}

		if(triangles->texcoords) {	
			glTexCoordPointer(2, GL_FLOAT, 0, triangles->texcoords->values);
		} else {

		}

		glDrawArrays(GL_TRIANGLES, 0, triangles->count * 3);

		glDisableClientState(GL_VERTEX_ARRAY);
	}

	glPopMatrix();
}

void RCBC_GL_Draw_Nodes(RCBCNode* node) {

	while(node) {
		glPushMatrix();
		RCBC_GL_Draw_Node(node);

		if(node->child) {
			RCBC_GL_Draw_Node(node->child);
		}
		glPopMatrix();
		node = node->next;
	}
}

int RCBC_GL_Draw(RCBCThing* thing) {
	RCBCNode* head = thing->visual_scene;

	/* Draw axis */
	/*glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();*/

	RCBC_GL_Draw_Nodes(head);
	return 0;
}
